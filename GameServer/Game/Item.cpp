#include "../Addr.h"
#include "Item.h"

#pragma region CItem

CItem::CItem()
{
	((void(*)())Addr::Item::Constructor)();
}

int CItem::tab(void)
{
	return ((int(*)(CItem*))Addr::Item::tab)(this);
}

int CItem::row(void)
{
	return ((int(*)(CItem*))Addr::Item::row)(this);
}

int CItem::col(void)
{
	return ((int(*)(CItem*))Addr::Item::col)(this);
}

LONGLONG CItem::Count(void) const
{
	return ((int(*)(const CItem*))Addr::Item::Count)(this);
}

bool CItem::IsRangeWeapon(void)
{
	return ((bool(*)(CItem*))Addr::Item::IsRangeWeapon)(this);
}
bool CItem::IsMagicWeapon(void)
{
	return ((bool(*)(CItem*))Addr::Item::IsMagicWeapon)(this);
}
int CItem::GetUsingStat(void)
{
	return ((int(*)(CItem*))Addr::Item::GetUsingStat)(this);
}
bool CItem::IsWeaponType(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsWeaponType)(this);
}
bool CItem::IsArmorType(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsArmorType)(this);
}
bool CItem::IsAccessary(void)
{
	return ((bool(*)(const CItem*))Addr::Item::IsAccessary)(this);
}
bool CItem::IsPet(void)
{
	return ((bool(*)(const CItem*))Addr::Item::IsPet)(this);
}
bool CItem::IsAPet(void)
{
	return ((bool(*)(const CItem*))Addr::Item::IsAPet)(this);
}
bool CItem::IsUsedPlatinumSpecial(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsUsedPlatinumSpecial)(this);
}
bool CItem::IsUpgradeRune(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsUpgradeRune)(this);
}
bool CItem::CanExchange(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanExchange)(this);
}
bool CItem::CanDrop(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanDrop)(this);
}
bool CItem::CanSellToShop(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanSellToShop)(this);
}
bool CItem::CanKeepStash(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanKeepStash)(this);
}
bool CItem::CanUpgrade(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanUpgrade)(this);
}
bool CItem::CanBloodGem(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanBloodGem)(this);
}
bool CItem::CanWear(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::CanWear)(this);
}
bool CItem::IsBelong(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsBelong)(this);
}
bool CItem::IsOriginItem(void) const
{
	return ((bool(*)(const CItem*))Addr::Item::IsOriginItem)(this);
}
int CItem::GetBelongType(void) const
{
}
int CItem::GetOriginIndex(int) const
{
}
int CItem::GetOriginLevel(int) const
{
}
int CItem::GetOriginOptionCnt(void) const
{
}
int CItem::GetOriginSkillCnt(void) const
{
}
bool CItem::IsRareItem(void) const
{
}
void CItem::IdentifyRareOption(void)
{
}
int CItem::GetItemLevel(bool) const
{
}
int CItem::GetItemPlus(void) const
{
}
bool CItem::CanCompositeEquipItem(void) const
{
}
bool CItem::IsJewelType(void) const
{
}
bool CItem::IsStableType(void) const
{
}
bool CItem::IsSocketItem(void) const
{
}
#pragma endregion

#pragma region  CItemList

CItem* CItemList::CreateDBItem( int idx, char wearPos, int plus, int flag, int used, int used_2, const char* serial, long long count, short* option, const char* arg1, int* arg2)
{
	return ((CItem*(*)(CItemList *, int, char, int, int ,int ,int, const char*, long long, short*, const char*, int*))Addr::ItemList::CreateDBItem_1)(this, idx, wearPos, plus, flag, used, used_2, serial, count, option, arg1, arg2);
}

CItem* CItemList::CreateDBItem( int idx, char wearPos, int plus, int flag, int used, int used_2, const char* serial, long long count, short* option, char arg1, int* arg2, int* arg3)
{
	return ((CItem*(*)(CItemList *, int, char, int, int ,int ,int, const char*, long long, short*, char, int*, int*))Addr::ItemList::CreateDBItem)(this, idx, wearPos, plus, flag, used, used_2, serial, count, option, arg1, arg2, arg3);
}


CItem* CItemList::CreateItem(int idx, char wearPos, int plus, int flag, long long count)
{
	return ((CItem*(*)(CItemList *, int, char, int, int , long long))Addr::ItemList::CreateItem)(this, idx, wearPos, plus, flag, count);
}

CItem* CItemList::CreateAutoGiveDBItem(int idx, char wearPos, int plus, int flag, int used, int used_2, const char* serial, long long count, short* option, char arg1, int* arg2)
{
	return ((CItem*(*)(CItemList *, int, char, int, int ,int ,int, const char*, long long, short*, char, int*))Addr::ItemList::CreateAutoGiveDBItem)(this, idx, wearPos, plus, flag, used, used_2, serial, count, option, arg1, arg2);
}

bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row = -1, int col = -1)
{
	return ((bool(*)(CPC*, CItem*, bool, bool, int , int))Addr::ItemList::AddToInventory)(ch, item, bSendWarning, bCheckWeight, row, col);
}

bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count)
{
	return ((bool(*)(CPC*, CItem*, LONGLONG))Addr::ItemList::DecreaseFromInventory)(ch, item, count);
}

bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot)
{
	return ((bool(*)(CPC*, int, int, int , bool, bool))Addr::ItemList::RemoveFromInventory)(ch, tab, row, col, bFreeMem, removeQuickSlot);
}

bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot)
{
	return ((bool(*)(CPC*, CItem*, bool, bool))Addr::ItemList::RemoveFromInventory_1)(ch, item, bFreeMem, removeQuickSlot);
}

void ItemUpdateMsg(CNetMsg& msg, CItem* item, LONGLONG changeCount)
{
	return ((void(*)(CNetMsg&, CItem*, LONGLONG))0x0809D8C6)(msg, item, changeCount);
}

void ItemAddMsg(CNetMsg& msg, CItem* item)
{
	return ((void(*)(CNetMsg&, CItem*))0x0809D6AD)(msg, item);
}

void SysFullInventoryMsg(CNetMsg& msg, char tab)
{
	return ((void(*)(CNetMsg&, char))0x080A0615)(msg, tab);
}

#pragma endregion