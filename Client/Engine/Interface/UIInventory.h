// ----------------------------------------------------------------------------
//  File : UIInventory.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIINVENTORY_H_
#define	UIINVENTORY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIIcon.h>
#include <Engine/Interface/UIEventPopup.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Help/Util_Help.h>

typedef CUIIcon*	btnAllItems[INVENTORY_TAB_MAX][ITEM_COUNT_IN_INVENTORY_NORMAL];	// Slot items

// Define item type tab
enum ItemTab
{
	ITEM_TAB_NORMAL	= 0,
	ITEM_TAB_CASH1,
	ITEM_TAB_CASH2,
	ITEM_TAB_TOTAL,
};

enum InventoryTab
{
	INVEN_TAB_NORMAL0 = 0,
	INVEN_TAB_NORMAL1,
	INVEN_TAB_NORMAL2,
	INVEN_TAB_NORMAL3,
	INVEN_TAB_NORMAL4,
	INVEN_TAB_CASH1,		// 상용화 인벤토리
	INVEN_TAB_CASH2,		// 상용화 인벤토리
	INVEN_TAB_TOTAL,
};

enum eNewItemTex
{
	NEW_EFFECT_ITEM = 0,
	NEW_EFFECT_BAG,
	NEW_EFFECT_BAG_CASH,
	NEW_EFFECT_TOTAL,
};

#define	ITEM_TAB_HEIGHT				58
#define	ITEM_TAB_WIDTH				21
#define	ITEM_TAB_GAP				3

// Define process requesting to lock inventory
enum InvenLockProcess
{
	LOCK_NONE = 0,
	LOCK_UPGRADE_ITEM,
	LOCK_BLOOD_OPTION,
	LOCK_REFINE,
	LOCK_COMPOUND,
	LOCK_GW_MIX,
	LOCK_MIX,
	LOCK_MIXNEW,
	LOCK_MASTERSTONE,
	LOCK_PROCESS,
	LOCK_PRODUCT,
	LOCK_PRODUCT_ITEM,
	LOCK_REMISSION,
	LOCK_EXCHANGE,
	LOCK_SHOP,
	LOCK_CHANGEWEAPON,
	LOCK_PERSONAL_SHOP,
	LOCK_WAREHOUSE,
	LOCK_QUEST,
	LOCK_WARP,
	LOCK_PRESSCORPS,
	LOCK_SOCKETSYSTEM,
	LOCK_SENDMAIL,
	LOCK_REFORMSYSTEM,
	LOCK_GUILDSTASH,
	LOCK_AFFINITY_DONATION,
	LOCK_MYSTERIOUSBEAD,
	LOCK_AUCTION,
	LOCK_PET_STASH,
	LOCK_MESSAGEBTN,
#ifdef DURABILITY
	LOCK_DURABILITY,
#endif	//	DURABILITY
	LOCK_TRADE,
	LOCK_EXP_PET,
	LOCK_PET_TRAINING,
	LOCK_GPS,
	LOCK_ITEMCOLLECTION,
	LOCK_PET_FREE,
	LOCK_ITEM_COMPOSE,
	LOCK_INVEN_ARRANGE,
	LOCK_INVEN_ITEM_DEL,
};

// Define max char and line of item informaion 
#define	MAX_ITEMINFO_CHAR			26

// Define text position
#define	INVEN_TITLE_TEXT_OFFSETX	25
#define	INVEN_TITLE_TEXT_OFFSETY	5
#define	ITEMTAB_TEXT_OFFSETY		147
#define	INVEN_MONEY_TEXT_POSX		211
#define	INVEN_MONEY_TEXT_POSY		182
#define	INVEN_WEIGHT_TEXT_POSX		82
#define	LEASE_INFO_TEXT_OFFSETX		180

// Define size of inventory
#define	INVENTORY_WIDTH				256
#define	INVENTORY_HEIGHT			408
#define	INVENTORY_WEIGHT_WIDTH		62

// define Ineventory Type Tab(General/Costume)
#define INVEN_TAB_GENERNAL			0
#define INVEN_TAB_COSTUME			1

// Inventroy Type Tab Size
#define INVENTORY_TYPE_TAP_WIDTH	34
#define INVENTORY_TYPE_TAP_HEIGHT	22

enum eInvenSlot
{
	INVENSLOT_NUM1 = 0,
	INVENSLOT_NUM2,
	INVENSLOT_NUM3,
	INVENSLOT_CASH1,
	INVENSLOT_CASH2,
	INVENSLOT_MAX,
};

enum eERR_MSG
{
	eERR_NONE = 0,
	eERR_NOT_OPEN_CASH_BAG,
	eERR_FLAG_COMPOSITION,
	eERR_MAX
};

// ----------------------------------------------------------------------------
// Name : CUIInventory
// Desc :
// ----------------------------------------------------------------------------

class CUIInventory : public CUIWindow
{
protected:
	struct INVEN_LOCK_ITEMINFO
	{
		SWORD	TargetTab;
		SWORD	TargetInvenIdx;
		SWORD	UseInvenTab;
		SWORD	UseInvenIdx;
		SBYTE	TargetWear_Pos;
		INDEX	TargetItem_Plus;
		INDEX	UseItem_Level;
		SLONG	TartetWearPos;
		SLONG	TartetVirIdx;
		SLONG	UseVirIdx;
	};

	enum IN_LOCK_ITEMINFO_TYPE
	{
		IN_LOCK_REFINE,
		IN_LOCK_OPTION_ADD,
		IN_LOCK_OPTION_DEL,
		IN_LOCK_DIVIDE,
		IN_LOCK_NONE,
	};

	// Controls
	CUIButton			m_btnClose;								// Close button
	BOOL				m_bLockInven;							// If inventory is locked or not
	BOOL				m_bLockArrange;							// If arrange button is locked or not
	bool				m_bLockSelect;							// 아이템 선택 막기.
	InvenLockProcess	m_eLockInvenProcess;					// Process requesting to lock inventory
	BOOL				m_bShowToolTip;							// If tool tip is shown or not
	BOOL				m_bShowWearSlotToolTip;
	CTString			m_strToolTip;							// String of tool tip
	CTString			m_strWearSlotToolTip;

	// Items
	InventoryTab		m_InvenCurrentTab;						//
	CUIIcon*			m_pIconsWearing[WEAR_TOTAL];				// Wearing items
	// UI개편  [10/10/2012 Ranma]	
	btnAllItems			m_abtnItems;
	BOOL				m_bSlot[INVENSLOT_MAX];
	CUIWindow*			m_pInvenSlot[INVENSLOT_MAX];

	// Items info add	
	int					m_nUseTab;
	int					m_InvenIndex;

	// Item information
	int					m_nSelWearItemID;						// Selected weating item
	int					m_nSelSlotItemID[INVEN_TAB_TOTAL];		// Selected slot item
	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	UIRect				m_rcItemInfo;							// Item information region
	CTString			m_strMyMoney;							// String of current money
	CTString			m_strWeight;							// String of current weight
	CTString			m_strNumItems;							// String of current items count
	COLOR				m_colWeight;							// Color of weight string
	int					m_iItemServerIndex;						// for booster upgrade

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	SLONG				m_slCompositeUniIdx;					// 조합된 의상 아이템 Uniidx
	SLONG				m_slCompositePlus;						// 조합된 의상 아이템 Plus
	SBYTE				m_sbCompositeRow;						// 조합된 의상 아이템 Row
	SBYTE				m_sbCompositeCol;						// 조합된 의상 아이템 Col

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcWearing;							// Region of wearing
	UIRect				m_rcItemTypeTab[2];						// Region of item type tab
	UIRect				m_rcItemSlot;							// Region of item slot0
	UIRect				m_rcToolTip;							// Region of tool tip
	UIRect				m_rcWearSlotToolTip;

	CUIRectSurface		m_NewInvenSurface;
	CUIRectSurface		m_WearInvenSurface;						// 장비창 배경
	CUIRectSurface		m_NewItemEffectSurface;					// 새로 얻은 아이템 깜빡이 이펙트의 이미지
	CTextureData		*m_ptdWearBackTexture;					// 장비창 텍스쳐

	BOOL				m_bNewItem[INVEN_ONE_BAG];
	BOOL				m_bStartEffectItem[INVEN_ONE_BAG];
	int					m_nPlayCount[INVEN_ONE_BAG];

	BOOL				m_bNewItemBag[INVENSLOT_MAX];
	BOOL				m_bStartEffectBag[INVENSLOT_MAX];
	int					m_nBagPlayCount[INVENSLOT_MAX];

	DWORD				m_nEffectTime;		// 업데이트를 위한 타임
	
	CUIButton			m_btnTab[INVEN_TAB_TOTAL];
	CUIButton			m_btnTrashcan;							// 휴지통 버튼
	CUIButton			m_btnArrange;							// 정렬 버튼
	
	std::vector<CTString>	m_vecItemName;						// [090701: selo] 아이템의 이름을 저장한다(아이템의 이름이 한줄을 넘어가는 것 때문에 이것이 존재)

	INVEN_LOCK_ITEMINFO	m_InLockItemInfo[IN_LOCK_NONE];

	CTextureData		*m_ptdCostumeTexture;						// 코스튬2 인벤토리 배경 텍스쳐
	int					m_iInventoryType;							// 현재 선택된 인벤토리 탭 타입 (General : 0 / Costume : 1)
	CUIIcon*			m_pIconsCostumeWearing[WEAR_COSTUME_TOTAL];	// 코스튬2 Wearing Item Button
	UIRect				m_rcInventoryTab[2];						// 코스튬2 탭 영역
	UIRectUV			m_rcCostumeTab;								// 코스튬2 탭 UV 영역
	CUIButton			m_btnInvenTab[2];							// 코스튬2 탭 버튼
	CUIRectSurface		m_kRSCostumeBackground;

	// [2011/01/18 : Sora] 출석 이벤트
	CUIEventPopUp	m_EventPopUp;

	CUIIcon*			m_pIconCurSelectItem;
	CUIImage*			m_pImgSelect;

protected:
	// Internal functions
	void	RenderItems();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );
	void	ShowWearGuideToolTip( BOOL bShow, eWEAR_TYPE eType = WEAR_TYPE_NONE, int WearPos = -1 );
	void	ShowWearTabToolTip( BOOL bShow, int nToolTipID = -1 );
	void	ShowArrangeToolTip( BOOL bShow );

	// Command functions
	void	UpgradeItem(SLONG nTargetPos, SLONG nTargetVirIndex, SWORD nRefineTab, SWORD nRefineIndex);

	void	OptionAddItem( SBYTE sbWearPos, SLONG slIndex, SWORD nTab, SWORD inven_idx, SLONG VirIdx );
	void	OptionDelItem( SBYTE sbWearPos, SLONG slIndex, SWORD nTab, SWORD inven_idx, SLONG VirIdx );
public:
	// Network message functions ( send )
	void	SendArrangeItem();
	void	SendSwapSlotItems( int nTab0, int inven_idx0, int nTab1, int inven_idx1 );
	void	SendUpgradeItem( INDEX iProtect =-1);
	void	SendItemLvDown();

	void	SendOptionAddItem();
	void	SendOptionDelItem();

	void	SetCashBagExpireTime( int type );
	int		IsCashInvenMoveItem( int inven_tab, int inven_idx = -1, int target_tab = -1, int target_idx = -1);
	void	ErrMessage(int errCode);
	void	CashInvenLockOn();
public:
	CUIInventory();
	~CUIInventory();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	FocusLeave();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Get row, col of warehouse item
	void	GetLocationOfNormalItem( int nUniIndex, SWORD& nTab, SWORD& inven_idx );

	//Get item info(tab,indenIdx) of DoubleClicked item
	void	GetUseItemSlotInfo(int &nTab, int &inven_idx);
	void	SetUseItemSlotInfo(int nTab, int inven_idx);
	
	// find
	SQUAD	GetItemCount(int iItemIndex, bool bCompositionItem = true);
	SQUAD	GetItemAll();

	// find function. [9/22/2009 rumist]
	BOOL	GetItemSlotInfo( int iItemIndex, int &nTab, int &inven_idx );
	
	// Lock
	void	Lock( BOOL bLock, BOOL bOnlyArrange, InvenLockProcess eWhichProcess )
	{
		if( !bLock && eWhichProcess != LOCK_NONE && m_eLockInvenProcess != eWhichProcess )
			return;

		m_bLockArrange = bLock;
		if( !bOnlyArrange )
			m_bLockInven = bLock;

		bLock ? m_eLockInvenProcess = eWhichProcess : m_eLockInvenProcess = LOCK_NONE;
	}
	BOOL	IsLocked() const { return m_bLockInven; }
	BOOL	IsLockedArrange() const { return m_bLockArrange; }
	bool	IsLockSelect()	{ return m_bLockSelect;	}
	void	SetLockSelect(bool bLock)	{ m_bLockSelect = bLock;	}
	InvenLockProcess getLocked()		{ return m_eLockInvenProcess; }
	void	ShowLockErrorMessage();

	// Toggle visible
	void	ToggleVisible();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( send )
	void	SendUseSlotItem( int nTab, int inven_idx, SBYTE sbWearType = -1 );

	// Network message functions ( receive )
	void	InitInventory( int nTab, int inven_idx, int nUniIndex, int nIndex, int nWearingType );
	void	InitWearBtn(int nIndex, int nUniIdex, int nWearingType);
	void	UpgradeItem( SBYTE sbResult );
	void	UpgradeItem( SBYTE sbResult, SBYTE sbplus );
	void	ClearAllItems( SBYTE sbTab );
	void	OptionAddRep( SBYTE sbResult );
	void	OptionDelRep( SBYTE sbResult );
	
	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	BOOL	StartSkillDelay( int nIndex );
	BOOL	GetSkillDelay( int nIndex );
	
	LONG	GetItemIndex( int nTab, int inven_idx );
	LONG	GetItemUniIndex( int nTab, int inven_idx );

	// [070824: Su-won] REFINE_PROTECT
	// 제련을 시도할 아이템의 +수치를 얻음
	SLONG	GetUpgradeItemPlus();

	INDEX	GetMySlotItemsCount(void);

	void	ShowItemDestroyMessage();
	BOOL	FindEmptyabtnItem(int nTab, int &inven_idx);
	
	// [090608: selo] 퀘스트 아이템 사용제한 체크
	BOOL	IsRestritionItem(int nTab, int inven_idx);
	
	// [091028 sora] 아이템 업그레이드 추가
	void	UpgradeCompositedItem(int inven_idx);

	BOOL	IsCostumeInventory()	//현재 선택된 인벤토리가 일반 인벤토리인지 코스튬 인벤토리인지 체크 ( return value : TRUE->코스튬인벤, FALSE->일반인벤)
	{ 
		return (m_iInventoryType == INVEN_TAB_COSTUME);
	}

	void	SetInventoryType(int iType);	// 인벤토리 탭 타입 설정 (General/Costume)
	void	InitCostumeInventory(INDEX iUni_index, INDEX iDB_index, SBYTE sbWear_type);
	
	CUIIcon*		GetItemIcon(int nUniqueIndex);
	CItems*			GetItems(int nUniqueIndex);

	// 아이템이 인벤토리 내 존재 하는지 검사
	BOOL			GetItemVisble( int nUniqueIndex );
	// 인벤토리에 빈 슬롯이 있는지 검사
	BOOL			IsInventoryFull();

	// [2011/01/18 : Sora] 출석 이벤트
	CUIEventPopUp* GetEventPopUp() { return &m_EventPopUp; }

	// get pet info in wearing m_abtnCostumeWearing
	const bool		IsWearing( const eEquipment _wearPos )			{ return m_pIconsWearing[_wearPos]->IsEmpty(); }
	bool			IsWearingItem(int nDBIndex);	// 아이템을 장비하고 있는지 검사
	CUIIcon*		GetWearingBtn( const eEquipment _wearPos )		{ return m_pIconsWearing[_wearPos];				}
	CUIIcon*		GetWearingBtnforSlot( const int _wearPos )		{ return m_pIconsWearing[_wearPos];				}
	CUIIcon*		GetCostumeWearingBtnforSlot( const int _wearPos )		{ return m_pIconsCostumeWearing[_wearPos];				}
	
	//added by sam 11/02/01 
	void InitOneSuitItemInfo ( int nTab, int inven_idx, INDEX iDB_index, INDEX iUni_index, INDEX sbWear_type );

	void SetTabSlotInfo(BOOL bSlot, int nSlotID) { m_bSlot[nSlotID] = bSlot; }
	
	CUIRectSurface* GetInvenSurface() { return &m_NewInvenSurface; }

	int		GetSelWearItemID() { return m_nSelWearItemID; }	
	void	SetSelWearItemID(int nWearID)	{ m_nSelWearItemID = nWearID; }
	void	SetCurSelectItem(CUIIcon* abtn);
	void	UpdateSelectItem();
	CUIIcon*	GetCurSelectItem() { return m_pIconCurSelectItem; }

	void	ShowDivideMsgBox(SWORD nTab, SWORD nInvenIdx);
	bool	IsDivideItem(SWORD nTabS, SWORD nIdxS, SWORD nTabT, SWORD nIdxT);
	
	// 깜빡이 이펙트
	void	UpdateNewItemEffect();

	void	ClearNewItemEffect();
	void	ClearNewBagEffect();

	void	SetNewItemBagEffect(BOOL bShowEffect, eInvenSlot eBagPos);
	void	SetNewItemEffect(BOOL bShowEffect, int InvenIdx);

	void	SetStartEffectItem(int InvenIdx);
	void	SetStartEffectBag(eInvenSlot eBagPos);

	BOOL	IsPlayNewItemEffect(int InvenIdx);
	BOOL	IsCheckNewItemEffectPlayCount(int InvenIdx);
	BOOL	IsPlayNewBagEffect(eInvenSlot eBagPos);
	BOOL	IsCheckNewBagEffectPlayCount(eInvenSlot eBagPos);
	void	DivideItemCallback();
	void	SetIconNewItem(int nTab, int idx, bool bOn);

	__int64 GetItemCountByUniIdx( int nUniqueIndex );
	
	// 인덱스에 해당하는 동일한 아이템중 결합되지 않은 아이템이 있는지 검사
	bool	IsNotCompositionItem( int nItemDBIdx);
private:
	int		GetCurTabSlotCount(int tab);	// 캐시 인벤 추가로 인해 normal탭은 슬롯이 100개, 캐시는 25개만 사용

	// 장착 창 배경 그리기 (직업별로 다르게 나와야 함)
	void	SendSortInventory();

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
};


#endif	// UIINVENTORY_H_

