#ifndef __ANPCREGENINFO_H__
#define __ANPCREGENINFO_H__

class CNPCRegenInfo {
     int m_index;
     int m_npcIdx;
     int m_zoneNo;
     float m_regenX;
     int m_regenY;
     float m_regenZ;
     float m_regenR;
     int m_regenSec;
     int m_totalNum;
     bool m_bAlive;
     int m_lastDieTime;
     int m_numRegen;
     int m_paramHP;
     bool m_bRaidMoveRandom;
     bool m_bRegen;
   public:
     CNPCRegenInfo(void);
     ~CNPCRegenInfo();
     void SetHPParam(int);
     void clean(void);
 };

class CArea;

class CNPCRegenList {
     int m_type;
     CNPCRegenInfo *m_infoList;
     int m_nCount;
     int m_wPos;
   public:
     CNPCRegenList(void);
     ~CNPCRegenList();
     void Init(void);
     void SetCount(int);
     void AddRegenInfo(int, int, int, float, int, float, float, int, int, bool);
     void AddRegenInfo(CNPCRegenInfo *);
     bool Find(int, int, bool, float, int, float);
     bool DelRegenInfo(CNPCRegenInfo *);
     bool SetRegened(CArea *, int, int, float, int, float);
     CNPCRegenInfo * GetNpcRegenInfo(int);
 };

#endif
