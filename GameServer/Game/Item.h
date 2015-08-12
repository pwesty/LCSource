#if !defined(AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_)
#define AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CPos.h"
#include "Option.h"
#include "RareOptionProto.h"
#include "Mutex.h"
#include "SetItemProto.h"
#include "Socket.h"
#include "NetMsg.h"
#include "LCList.h"
#include "Affinity.h"
#include <map>
#include <set>

class CArea;

class CItemProto {
           public:
             int m_index;
             int m_typeIdx;
             int m_subtypeIdx;
             CLCString m_name;
             int m_jobFlag;
             int m_flag;
             int m_maxUse;
             int m_fame;
             int m_num0;
             int m_num1;
             int m_num2;
             int m_num3;
             int m_num4;
             int m_set0;
             int m_set1;
             int m_set2;
             int m_set3;
             int m_set4;
             int m_grade;
             int m_weight;
             LONGLONG m_price;
             int m_wearing;
             int m_materialIndex[10];
             int m_materialCount[10];
             int m_needSSkillIndex;
             int m_needSSkillLevel;
             int m_needSSkillIndex2;
             int m_needSSkillLevel2;
             int m_nRareIndex[10];
             int m_nRareProb[10];
             int m_level;
             int m_level2;
			 int m_MaxOriginVariation[6];
           public:
             CLCList<CAffinityProto*> m_affinityList;

             CItemProto(void);
             ~CItemProto();
         int getItemIndex(void) const
		 {
			return ((int(*)(const CItemProto *))0x0806FEBE)(this);
		 }
         int getItemTypeIdx(void) const;
         int getItemSubTypeIdx(void) const;
         CLCString getItemName(void) const;
         int getItemJobFlag(void) const;
         int getItemFlag(void) const;
         int getItemMaxUse(void) const;
         int getItemFrame(void) const;
         int getItemNum0(void) const;
         int getItemNum1(void) const;
         int getItemNum2(void) const;
         int getItemNum3(void) const;
         int getItemNum4(void) const;
         int getItemSet0(void) const;
         int getItemQuestZone(void) const;
         int getItemSet1(void) const;
         int getItemQuestX(void) const;
         int getItemSet2(void) const;
         int getItemQuestZ(void) const;
         int getItemSet3(void) const;
         int getItemQuestY(void) const;
         int getItemSet4(void) const;
         int getItemQuestRange(void) const;
         int getItemGrade(void) const;
         int getItemWeight(void) const;
         LONGLONG getItemPrice(void) const;
         int getItemWearing(void) const;
         int getItemMaterialIndex(int) const;
         int getItemMaterialCount(int) const;
         int getItemNeedSSkillIndex(void) const;
         int getItemNeedSSkillLevel(void) const;
         int getItemNeedSSkillIndex2(void) const;
         int getItemRareIndex(int) const;
         int getItemRareProb(int) const;
         int getItemLevel(void) const;
         int getItemLevel2(void) const;
         bool IsRareItem(void) const;
         bool CanBloodGem(void) const;
         int GetItemProtoLevel(void) const;
         int GetItemProtoLevel2(void) const;
         bool IsOriginItem(void) const;
         int GetSetIndex(void) const;
         int GetBelongType(void) const;
         int GetOriginIndex(int) const;
         int GetOriginLevel(int) const;
         int GetOriginOptionCnt(void) const;
         int GetOriginSkillCnt(void) const;
     };

class CItem {
       private:
		   int bla;
         int m_tab_idx;
         int m_row_idx;
         int m_col_idx;
         LONGLONG m_count;
       public:
         const CItemProto *m_itemProto;
         int m_index;
         int m_idNum;
         char m_wearPos;
         int m_plus;
         int m_plus2;
         int m_flag;
         CLCString m_serial;
         int m_used;
         int m_used_2;
         int m_MercenaryIndex;
         COption m_option[5];
         int m_nOption;
         const CRareOptionProto *m_pRareOptionProto;
         int m_nRareOptionIndex;
         int m_nRareOptionBit;
         int m_nCompositeItem;
         const CSetItemProto *m_pSetItemProto;
         int m_nSetWearCnt;
         int m_nSetOption;
         CArea *m_pArea;
         CPos m_pos;
         int m_cellX;
         int m_cellZ;
         CItem *m_pCellPrev;
         CItem *m_pCellNext;
         int m_groundPulse;
         int m_preferenceIndex;
         CSocketList m_socketList;
		 int m_OriginVar[6];

         CItem(void);
         ~CItem();
         int tab(void);
         int row(void);
         int col(void);
         void SetDropNpc(int);
         int GetDropNpc(void);
         LONGLONG Count(void) const;
         bool IsRangeWeapon(void);
         bool IsMagicWeapon(void);
         int GetUsingStat(void);
         bool IsWeaponType(void) const;
         bool IsArmorType(void) const;
         bool IsAccessary(void);
         bool IsPet(void);
         bool IsAPet(void);
         bool IsUsedPlatinumSpecial(void) const;
         bool IsUpgradeRune(void) const;
         bool CanExchange(void) const;
         bool CanDrop(void) const;
         bool CanSellToShop(void) const;
         bool CanKeepStash(void) const;
         bool CanUpgrade(void) const;
         bool CanBloodGem(void) const;
         bool CanWear(void) const;
         bool IsLent(void) const;
         bool IsBelong(void) const;
         bool IsOriginItem(void) const;
         int GetBelongType(void) const;
         int GetOriginIndex(int) const;
         int GetOriginLevel(int) const;
         int GetOriginOptionCnt(void) const;
         int GetOriginSkillCnt(void) const;
         bool IsRareItem(void) const;
         void IdentifyRareOption(void);
         int GetItemLevel(bool) const;
         int GetItemPlus(void) const;
         bool CanCompositeEquipItem(void) const;
         bool IsJewelType(void) const;
         bool IsStableType(void) const;
         bool IsSocketItem(void) const;
     };

class CItemList {
   private:
     CMutexObject m_mutexobj;
   public:
     int m_nCount;
     int m_virtualIndex;
     CItemProto *m_protoItems;
     CItemProto *m_moneyItem;
     CItemProto *m_normalRefineItem;
     CItemProto *m_specialRefineItem;
     CItemProto *m_sTicketItem1;
     CItemProto *m_sTicketItem2;
     std::set<int, std::less<int>, std::allocator<int> > m_listLendItem;
   private:
	 typedef std::map<int, CItemProto*, std::less<int>, std::allocator<std::pair<int const, CItemProto*> > > map_t;
     map_t map_;

     CItemList(const CItemList &);
   public:
     CItemList(void);
   private:
     CItemList & operator=(const CItemList &);
   public:
     ~CItemList();
     bool Load(void);
     CItemProto * FindIndex(int);
     CItem * CreateDBItem(int, char, int, int, int, int, const char *, long long, short *, char, int *, int *);
     CItem * CreateDBItem(int, char, int, int, int, int, const char *, long long, short *, const char *, int *);
     CItem * CreateAutoGiveDBItem(int, char, int, int, int, int, const char *, long long, short *, char, int *);
     CItem * CreateItem(int, char, int, int, long long);
   protected:
     int GetNextVirtualIndex(void);
 };

struct CCtItemList {
   private:
     CItem *ctItem;
     int m_count;

   public:
     CCtItemList(void);
     ~CCtItemList();
     bool Load(int);
     CItem * GetItem(int);
     CItem * GetItem(CItem *);
     int GetCount(void);
 };

struct CCatalog {
   private:
     CCtItemList *m_ctItemList;
     int m_index;
     CLCString m_name;
     int m_type;
     int m_category;
     int m_subtype;
     int m_cash;
     int m_mileage;
     int m_toSell;
     int m_flag;
     bool m_enable;

   public:
     CCatalog(void);
     CCatalog(int);
     CCatalog(int, const char *, int, int, int, int, int, bool, int);
     ~CCatalog();
     void SetCatalog(int, const char *, int, int, int, int, int, bool, int, CCtItemList *);
     void SetPrice(int);
     int GetIndex(void);
     const char * GetName(void);
     int GetCategory(void);
     int GetSubtype(void);
     int GetFlag(void);
     int GetType(void);
     int GetCash(void);
     int GetMileage(void);
     int GetToSell(void);
     void SetToSell(int);
     int GetLimit(void);
     bool GetEnable(void);
     CCtItemList * GetCtItemList(void);
 };

struct CCatalogList {
   public:
     int m_typelist[30];
     int m_typeCount;
     int m_subtypelist[80];
     int m_subtypeCount;
     int m_recoveryCtid;
   private:
     CCatalog *catalog;
     int m_cCount;

   public:
     CCatalogList(void);
     ~CCatalogList();
     bool Load(void);
     bool Reload(void);
     bool ReloadPrice(void);
     CCatalog * Find(int);
     CCatalog * FindItemToCatalog(CItem *);
     CCatalog * FindItemToCatalog(int);
     void GetCashMoonCatalogList(CLCList<int> &, double, double);
     int GetCount(void);
     CCatalog * getCatalog(int);
   private:
     static int CompIndex(const void *, const void *);
 };

typedef struct __tagLuckyDrawBoxResult {
    int item_idx;
    LONGLONG count;
    int upgrade;
    int prob;
    int flag;
  public:
    __tagLuckyDrawBoxResult(void);
} LuckyDrawBoxResult;

typedef struct __tagLuckyDrawBoxNeed {
    int item_idx;
    LONGLONG count;
  public:
    __tagLuckyDrawBoxNeed(void);
} LuckyDrawBoxNeed;


typedef struct __tagLuckyDrawBoxData {
    int index;
    int random;
    CLCString strName;
    int nNeedCount;
    int nResultCount;
    LuckyDrawBoxNeed *needList;
    LuckyDrawBoxResult *resultList;
  public:
    __tagLuckyDrawBoxData(void);
} LuckyDrawBoxData;


struct CLuckyDrawBox {
   protected:
     LuckyDrawBoxData *m_DataList;
     int m_nCount;

   public:
     CLuckyDrawBox(void);
     ~CLuckyDrawBox();
     bool Load(void);
     void RemoveAll(void);
     int GetCount(void);
     LuckyDrawBoxData * GetData(int);
 };

bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row, int col);
bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count);
bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot);
bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot);
bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row, int col);
void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum);

void ItemUpdateMsg(CNetMsg& msg, CItem* item, LONGLONG changeCount);

void ItemAddMsg(CNetMsg& msg, CItem* item);

void SysFullInventoryMsg(CNetMsg& msg, char tab);
#endif