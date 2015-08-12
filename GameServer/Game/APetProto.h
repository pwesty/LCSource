#ifndef __APET_PROTO_H__
#define __APET_PROTO_H__

#define HUMAN_PET_TYPE (1<<1)
#define BEAST_PET_TYPE (1<<2)

typedef struct _apetexpinfo {
       int nMaxAccExpParam1;
       int nMaxAccExpParam2;
       int nAccExpRateParam1;
       int nAccExpRateParam2;
       int nCooltime;
       int nCooltimeRate;
   } APET_EXP_INFO; 

typedef struct _evinfo {
       int nLevel;
       int nStm;
       int nFaith;
       int nStatType[2];
       int nEvPetIndex;
   } APET_EV_INFO; 

class CAPetProto {
       private:
         int m_nIndex;
         int m_nFlag;
         APET_EXP_INFO *m_pExpInfo;
       public:
         int m_Type;
         CLCString m_strName;
         int m_nStr;
         int m_nCon;
         int m_nDex;
         int m_nInt;
         int m_nItemIndex;
         int m_nMaxFaith;
         int m_nMaxStm;
         int m_nMaxHP;
         int m_nMaxMP;
         int m_nRecoverHP;
         int m_nRecoverMP;
         int m_nAISlot;
         int m_nAttack;
         int m_nDefence;
         int m_nMagicAttack;
         int m_nMagicDefence;
         int m_nHitPoint;
         int m_nAvoidPoint;
         int m_nMagicAvoidPoint;
         int m_nAttackSpeed;
         int m_nDeadly;
         int m_nCritical;
         int m_nAwful;
         int m_nStrong;
         int m_nNormal;
         int m_nWeek;
         int m_nDelay;
         int m_nBagicSkill1;
         int m_nBagicSkill2;
         int m_TansType;
         int m_nTransStart;
         int m_nTransEnd;
         int m_nMount[2];
         int m_nSummonSkill[2];
         int m_nSpeed[2];
         CLCList<_evinfo> m_listEvInfo;

         CAPetProto(void);
         ~CAPetProto();
         int Index(void);
         void Index(int);
         void InitValue(void);
         void SetFlag(int);
         bool IsFlag(int);
         const APET_EXP_INFO * GetExpInfo(void);
         void SetExpInfo(const APET_EXP_INFO *);
     };


class CAPet;

class CAPetProtoList {
       public:
         CAPetProto *m_pApetProtoList;
         int m_nCount;
         int m_virtualIndex;

         CAPetProtoList(void);
         ~CAPetProtoList();
         bool Load(void);
         CAPet * Create(CPC *, int, int);
         CAPetProto * FindProto(int);
       protected:
         static int CompIndex(const void *, const void *);
     };

#endif //__APET_PROTO_H__