#ifndef __MISSION_H
#define __MISSION_H

#include"LCList.h"
#include "LCString.h"

struct MOBTYPE {
     int npcidx;
     char count;
 };

struct MOBREGEN {
     char step;
     char typecount;
     MOBTYPE *mobtype;
   public:
     MOBREGEN(void);
     ~MOBREGEN();
 };

struct PENALTY {
     int index;
     int level;
 };

class CMissionCaseProto {
     int m_nIndex;
     int m_nNas;
     int m_nPoint;
     CLCString m_name;
     int m_nRegenCount;
     MOBREGEN *m_npcRegen;
     CLCList<PENALTY> m_listPanalty;
   public:
     bool LoadProto(void);
     const MOBREGEN * FindMobRegen(int);
     CMissionCaseProto(void);
     ~CMissionCaseProto();
 };

class CMIssionCaseList {
     int m_nCount;
     CMissionCaseProto *m_proto;
   public:
     CMIssionCaseList(void);
     ~CMIssionCaseList();
     CMissionCaseProto * FindProto(int);
     bool LoadList(void);
     static int CompIndex(const void *, const void *);
 };

#endif