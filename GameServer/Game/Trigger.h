#pragma once

#include "LCString.h"
#include "LCList.h"
#include "Conf.h"
#include "Config.h"
#include "NetMsg.h"
#include "Character.h"

class CArea;

class CRaidObjectDataProto {
       private:
         int m_nDBIndex;
         int m_nAssignZone;
         int m_nTodoOutput;
         int m_nObjectType;
         int m_nObjectIndex;
         char m_strObjectName[50];
         int m_nObjectData;
         int m_flag;

       public:
         CRaidObjectDataProto(void);
         ~CRaidObjectDataProto();
         int GetIndex(void);
         int GetAssignZone(void);
         int GetObjectIndex(void);
         int GetObjectData(void);
         int GetTodoOutput(void);
         int GetObjectType(void);
         int GetFlag(void);
         void SetIndex(int);
         void SetAssignZone(int);
         void SetObjectIndex(int);
         void SetObjectData(int);
         void SetObjectType(int);
         void SetName(const char *);
         void SetTodoOutput(int);
         void SetFlag(int);
     };

class CRaidDataProto {
       private:
         int m_nDBIndex;
         int m_nRaidNo;
         int m_nSceneNo;
         char m_strName[100];
         CRaidObjectDataProto *m_TodoProto;
         CRaidObjectDataProto *m_OutputProto[5];
         int m_nPreCond[3];
         int m_nDifficulty;
         int m_nRepeat;

       public:
         CRaidDataProto(void);
         ~CRaidDataProto();
         int GetIndex(void);
         int GetRaidNo(void);
         int GetSceneNo(void);
         int GetPreCond(int);
         CRaidObjectDataProto * GetTodo(void);
         CRaidObjectDataProto * GetOutput(int);
         void SetIndex(int);
         void SetRaidNo(int);
         void SetSceneNo(int);
         void SetPreCond(int, int);
         void SetName(const char *);
         void SetOutput(int, int);
         void SetTodo(int);
         void SetDifficulty(int);
         int GetDifficulty(void);
         void SetRepeat(int);
         int GetRepeat(void);
     };

class CRaidObjectList {
   private:
     int m_nCountRow;
     int m_nZoneNo;
     int *m_nObject;
     int *m_nObjectIndex;
     int *m_nState;

   public:
     CRaidObjectList(void);
     int GetObject(int);
     int GetObjectIndex(int);
     int GetObjectState(int);
     bool Load(int);
     int GetCountRow(void);
     int GetZoneNo(void);
     void ObjectReset(void);
     void SetObjectState(int, int, int);
     ~CRaidObjectList();
 };

class CRaidObjectData {
   private:
     CRaidObjectDataProto *m_proto;
     int m_nState;

   public:
     CRaidObjectData(void);
     void SetAttr(void);
     ~CRaidObjectData();
     void SetState(int);
     void SetProto(CRaidObjectDataProto *);
     int GetState(void);
     CRaidObjectDataProto * GetProto(void);
 };

class CRaidData {
   private:
     CRaidDataProto *m_proto;
     CRaidObjectData *m_Todo;
     CRaidObjectData *m_Output[5];
     int m_nState;

   public:
     CRaidData(void);
     ~CRaidData();
     void SetProto(CRaidDataProto *);
     void SetTodo(CRaidObjectData *);
     void SetOutput(int, CRaidObjectData *);
     void SetState(int);
     CRaidDataProto * GetProto(void);
     CRaidObjectData * GetTodo(void);
     CRaidObjectData * GetOutput(int);
     int GetState(void);
 };

class CAreaRaidInfoList {
   public:
     CRaidObjectList m_objectlist;
     CArea *m_pArea;
   private:
     CRaidData *m_RaidData;
     int m_nCount;
     int m_nSavePoint;

   public:
     CAreaRaidInfoList(void);
     int GetSavePoint(void);
     void ExcuteOutput(int, int, int, CPC *);
     void SetTheObjectState(int, int, int);
     void ExcuteTodo(int, int, int, CPC *);
     int LoadSavePointRaidInfo(int, int);
     void ResetRaidInfo(int);
     bool LoadRaidInfo(int, int);
     void SetCount(int);
     int GetCount(void);
     ~CAreaRaidInfoList();
 };

class CTrigger {
     int m_index;
     int m_triggerset_type;
     int m_trigger_touchID;
     int m_trigger_playID;
     int m_trigger_type;
     CLCString m_trigger_data;
     int m_nSaveLevel;
     int m_group_index;
     int m_nType_sub;
     int m_bContinue;
     int m_bDomino;
     int m_bPlayOnePC;
     bool m_bUsed;
     long m_TriggerTime;
     int m_nItem_index_ItemComposition;
     int m_nMeterial_IndexCount;
     int m_nMeterial_index[10];
     int m_nMeterial_count[10];
     int m_ItemDrop_index;
     int m_ItemDrop_percent;
     int m_Killed_Npc_Index;
     int m_Killed_Npc_Count;
     int m_NeedItem_IndexCount;
     int m_NeedItem_Index[10];
     int m_NeedItem_Count[10];
     int m_nStringIndex;
     int m_nSummonNpc_indexCount;
     int m_nSummonNpc_index[10];
     int m_nSummonNpc_count[10];
     int m_nUseDebuf_indexCount;
     int m_nUseDebuf_index[10];
     int m_Item_Index_removeNPC;
     int m_KilledNpc_Index_ForRemoveNpc;
     int m_KillCount;
     long m_TimeLimitSec;
     int m_nQuestIndex;
     int m_nGetItem_TotalCount;
     int m_nGetItem_Index[10];
     int m_nGetItem_Count[10];
     int m_nGetItem_Prob[10];
     int m_nMainDialogueString;
     int m_nNextDialogueString1;
     int m_nNextDialogueString2;
     int m_nNextDialogueString3;
     int m_nNextDialogueString4;
     int m_nNextDialogueString5;
     int m_nNextDialogueTriggerID1;
     int m_nNextDialogueTriggerID2;
     int m_nNextDialogueTriggerID3;
     int m_nNextDialogueTriggerID4;
     int m_nNextDialogueTriggerID5;
     CLCString m_strEffectName;
     int m_nTargetType;
     int m_nEffectNPCIndex;
   public:
     CTrigger(void);
     ~CTrigger();
     void SetTriggerTime(long);
     long GetTriggerTime(void);
     void CopyTriggerDate(CTrigger *);
 };

class CTrigger_List {
   public:
     CTrigger *m_Trigger;
     int m_nCount;
     CArea *m_pArea_UseTrigerEvent;
     int m_nNPC963_KilledCount;
     int m_flagTriggerEvent;

     int Check_TriggerFlag(int);
     void Set_TriggerFlag(int);
     void Reset_TriggerFlag(int);
     void InitTriggerList(void);
     bool Load_Trigger(void);
     CTrigger_List(void);
     ~CTrigger_List();
     bool ResetTriggerInfo(bool);
     bool SaveTriggerInfo(int, int);
     void LoadTriggerSaveInfo(int);
     bool LoadTriggerInfo(int);
     bool SyncForClient_TriggerInfo(CPC *);
     void TriggerTimerCheck(void);
     void SetArea(CArea *);
     CArea * GetArea(void);
   private:
     void Swap_Trigger(CTrigger *, CTrigger *);
 };

class CRaidDataProtoList {
   private:
     CRaidDataProto *m_RaidDataProto;
     int m_nCount;

   public:
     CRaidDataProtoList(void);
     int GetZoneCount(int, int);
     CRaidDataProto * Find(int);
     CRaidDataProto * FindUseArray(int);
     bool Load(void);
     int GetCount(void);
     ~CRaidDataProtoList();
 };

class CRaidObjectDataProtoList {
   private:
     CRaidObjectDataProto *m_RaidObjectDataProto;
     int m_nCount;

   public:
     CRaidObjectDataProtoList(void);
     CRaidObjectDataProto * Find(int);
     bool Load(void);
     ~CRaidObjectDataProtoList();
 };

class CRaidInfo {
   public:
     CRaidDataProtoList m_RaidDataProtoList;
     CRaidObjectDataProtoList m_RaidObjectDataProtoList;

     CRaidInfo(void);
     bool Load(void);
     ~CRaidInfo();
 };