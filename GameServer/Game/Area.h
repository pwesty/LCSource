#ifndef __AAREA_H__
#define __AAREA_H__

#include "Trigger.h"
#include "Env.h"
#include "Cell.h"
#include "MessageType.h"
#include "MonsterCombo.h"

class CZone;

class CGroundEffect2 {
       private:
         CLCList<EFFECTPOS> m_listEffect;
         CNPC *m_effectNPC[30];
         CArea *m_area;
         int m_zonenum;
         int m_pulseFirst;
         int m_pulseSecond;
         bool m_bEffectStart;

       public:
         CGroundEffect2(void);
         bool IsStarted(void);
         ~CGroundEffect2();
         bool Init(int, CArea *);
         void Start(void);
         void Stop(void);
         void Activity(void);
         CArea * GetArea(void);
       private:
         bool EffectReady(void);
         bool EffectFire(void);
         void Clean(void);
     };

class CArea {
   public:
     CGroundEffect2 m_GroundEffect2;
     CPC *m_AkanPCList[8];
     int m_nCountDown;
     int m_nObjectIndex;
     int m_nPauseMobActivity;
     int m_nPauseIndex;
     CNPC *m_pBossAppearNPC;
     int m_nDelayOpenGate;
     int m_nRaidRoomNo;
     bool m_bEnable;
     CZone *m_zone;
     int m_index;
     int m_size[2];
     int m_ycnt;
     CEnv ***m_env;
     CCell **m_cell;
     CNPCRegenList m_npcRegenList;
     CLCList<CNPCRegenInfo*> m_raidNPCRegenList;
     CLCList<int> m_NpcPortal_NpcList;
     CLCList<int> m_NpcPortal_MobList;
     CNPC *m_npcList;
     int m_nCountNPC;
     int m_pulseNPCAI;
     int m_pulseNPCRegen;
     int m_nJunkItems;
     int m_nMakeNPC;
     bool m_bBoss;
     int *m_regenList;
     int m_guildIndex;
     int m_nNPC_Killed_NPC;
   private:
     CNetMsg mMoveMsg;
     CNetMsg mAppMsg;
     CNetMsg mDisAppMsg;
     CNetMsg mTAppMsg;
     CNetMsg mTDisAppMsg;
   public:
     CMonsterCombo *m_monsterCombo;
     bool m_bUseTriggerEvent;
     CTrigger_List m_CTriggerList;
     int m_pulseProcTrigger;
     CAreaRaidInfoList m_RaidInfoList[2];
     int m_RaidDifficulty;

     CArea(void);
     ~CArea();
     void Init(CZone *, int, int, int, int);
     void PointToCellNum(float, float, int *, int *);
     void CharToCell(CCharacter *, char, int, int);
     void CharFromCell(CCharacter *, bool);
     bool MoveChar(CCharacter *, char, float, float, float, float, __tagMsgMoveType, CCharacter *);
   private:
     bool IsNotBlocked(char, float, float, float, float, bool, char &);
   public:
     bool IsNotBlocked(CCharacter *, CCharacter *, bool, char &);
     bool IsNotBlocked(CCharacter *, char, float, float, bool, char &);
     unsigned char GetAttr(char, float, float);
     float GetHeight(char, float, float);
     CCharacter * FindCharInCell(CCharacter *, int, __tagMsgCharType, bool);
     CNPC * FindProtoNPCInCell(float, float, int, bool, int, bool);
     CNPC * FindProtoNPCInCell(CCharacter *, int, bool, int, bool);
     CCharacter * FindChar(int, __tagMsgCharType);
     bool SendToCell(CNetMsg &, CCharacter *, bool, int);
     bool SendToCell(CNetMsg &, char, int, int, int);
     void SendToAllClient(CNetMsg &);
     void SendCellInfo(CPC *, bool);
     int GetPCCount(CCharacter *, int);
     int GetPCCount(int, int, int);
     int GetPCCount(void);
     void LoadNPC(void);
     void AddNPC(CNPC *);
     void DelNPC(CNPC *);
     CNPC * MobTimeCheck(CNPC *);
     void SetEnable(int, int);
     void SetDisable(void);
     void MobActivity(void);
     void MobRegen(void);
     bool RaidMobRegen(float, float, float, char);
     CCharacter * FindNearestTarget(CNPC *, float *);
     CCharacter * FindNearestTarget_Taming_or_Confused(CNPC *, float *);
     void MobActivity_Taming(CNPC *);
     void MobActivity_Confused(CNPC *);
     void Monster_synchronization(int);
     void MobActivity_Mercenary(CNPC *);
     void MobActivity_Totem(CNPC *);
     void MobActivity_Suicide(CNPC *);
     void ItemToCell(CItem *, char, int, int);
     void ItemFromCell(CItem *);
     CItem * DropItem(int, CCharacter *, int, int, long long, bool);
     CItem * DropItem(CItem *, CCharacter *);
     void GroundItemControl(void);
     CNPC * FindNearestFamily(CNPC *, int, int);
     void MoveAllChar(int, int);
     void RunComboZone(void);
     int CountPCInExtra(int, bool);
     int CountMobInCombo(void);
     void CloseComboZone(void);
     void Change_NpcRegenRaid(int, int);
     int GetAllClientCount_Area(void);
     void GotoAllClient_StartPoint(void);
     void ApplySkillToAllClient(CSkill *, bool &);
 };

void GoZone(CPC* ch, int zone, char ylayer, float x, float z);
void GoTo(CCharacter* ch, char y, float x, float z, float h, float r, char bHack = 0);
void GMGoZone(CPC* ch, CPC* pc);
#endif
