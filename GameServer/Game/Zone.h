#ifndef __AZONE_H__
#define __AZONE_H__

#include "MapAttr.h"
#include "Shop.h"
#include "LCList.h"
#include "LCString.h"
#include <map>

class CNPC;
class CArea;
class CItem;

struct NPC_REGEN_FOR_RAID {
     int m_npcIdx;
     float m_pos_x;
     float m_pos_z;
     float m_pos_h;
     float m_pos_r;
     int m_y_layer;
     int m_storeMiddle;
 };

class RECTF {
     float left;
     float top;
     float right;
     float bottom;
   public:
     RECTF(void);
 };

class TREASUREBOXINFO {
     int max;
     RECTF rect;
     CLCList<CNPC*> listNpc;
   public:
     TREASUREBOXINFO(void);
     ~TREASUREBOXINFO();
     int GetEmptyRegenCount(void);
     bool SetTreasureBoxNpc(CNPC *);
     void RemoveTreasureBoxNpc(CNPC *);
     void RemoveAllTreasureBoxNpc(void);
 };

class CNpc_Regen_Raid {
     int m_nCount_regen;
     NPC_REGEN_FOR_RAID *m_regenList_InstantDoungen;
   public:
     CNpc_Regen_Raid(void);
     ~CNpc_Regen_Raid();
 };

class CZone {
   public:
     int m_index;
     bool m_bRemote;
     CLCString m_remoteIP;
     unsigned short m_remotePort;
     int m_countY;
     CMapAttr *m_attrMap;
     CArea *m_area;
     int m_countArea;
     unsigned char m_weather;
     bool m_bCanMemPos;
     bool m_bCanSummonPet;
     bool m_bCanMountPet;
     int m_iContinent_no;
     int m_countZonePos;
     int **m_zonePos;
	 std::map<int, CShop*, std::less<int>, std::allocator<std::pair<int const, CShop*> > > map_;
     CShop *m_shopList;
     int m_nShopCount;
     int m_sellRate;
     int m_buyRate;
     CNpc_Regen_Raid *m_CNpcRegenRaidList;
     int m_MaxLevelNpcRegen;
   private:
     int m_flag;
   public:
     int m_treasureAreaRotate;
     int m_treasureAreaCnt;
     int m_treasureLinkZoneCnt;
     int *m_treasureLinkZoneIdx;
     TREASUREBOXINFO *m_pTreasureBoxInfo;

     CZone(void);
     ~CZone();
     bool LoadZone(int);
     bool LoadNPC(void);
     bool LoadShop(void);
     CShop * FindShop(int);
     void ChangeWeather(void);
     void ChangeTaxRate(int, int);
     int FindEmptyArea(void);
     int SetEnableArea(void);
     void SetDisableArea(int);
     bool IsPersonalDungeon(void);
     bool IsPartyRecallZone(void);
     bool IsSummonZone(void);
     bool IsGuildRoom(void);
     bool IsOXQuizRoom(void);
     bool IsComboZone(void);
     bool IsExtremeCube(void);
     bool IsPkZone(void);
     bool IsWarGroundZone(void);
     bool IsPersonalRaid(void);
     bool IsExpedRaidZone(void);
     bool IsPartyRaidZone(void);
     bool IsWarZone(int, int);
     int FindComboArea(int);
     int GetExtra(int, int);
     bool Load_NpcRegenRaidList(void);
     bool IsFieldZone(void);
   protected:
     static int CompIndex(const void *, const void *);
   public:
     int CheckZoneFlag(int);
     void SetTreasureDropFlag(void);
     void ResetTreasureDropFlag(void);
     int CheckTreasureDropFlag(void);
     void RemoveTreasureBoxNpc(CNPC *);
     bool SetTresaureMapInfo(CItem *);
     bool SummonTreaseureBoxNpc(int &, int &, int &);
     int NextAreaNum(void);
     bool IsTreasureDropZone(void);
 };

#endif
