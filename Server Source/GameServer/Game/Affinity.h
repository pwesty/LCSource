#ifndef _AFFINITY_H__
#define _AFFINITY_H__

#include <map>

class CPC;

class REWARDITEM {
     int allowpoint;
     int itemidx;
     int itemflag;
     int itemcount;
     int exp;
     int sp;
     int needPCLevel;
     int needItemIdx;
     int needItemCount;
} ;

class REWARDSTEP {
     int affinitynpc;
     int point;
 } ;

class __tagAffinityWork {
     int dataidx;
     int value;
 };

class CAffinityNPC {
     int m_npcidx;
     int m_shoppoint;
     int m_totalreward;
     REWARDITEM *m_reward;
     int m_flag;
   public:
     CAffinityNPC(void);
     ~CAffinityNPC();
     REWARDITEM * GetNextReward(int);
     bool LoadRewardItem(void);
     int GetFlag(int);
 };

class CAffinityProto {
   public:
     int m_index;
     CLCString m_name;
     int m_maxvalue;
     int m_nas;
     int m_needAffinIdx;
     int m_needAffinPoint;
     int m_needPCLevel;
     int m_needItemIdx;
     int m_needItemCount;
     int m_npcWorkCount;
     int m_itemWorkCount;
     int m_questWorkCount;
     int m_affinityNPCCount;
     __tagAffinityWork *m_npcWork;
     __tagAffinityWork *m_itemWork;
     __tagAffinityWork *m_questWork;
     CAffinityNPC *m_affinityNPC;

     CAffinityProto(void);
     ~CAffinityProto();
     int GetAffinityPointOfNPC(int);
     int GetAffinityPointOfQuest(int);
     int GetAffintiyPointOfItem(int);
     bool LoadAffinityWork(void);
     bool LoadAffinityNPC(void);
     CAffinityNPC * FindAffinityNPC(int);
 };

class CAffinity {
   public:
     CAffinityProto *m_proto;
     int m_affinityidx;
     int m_point;
     CLCList<REWARDSTEP*> m_rewardstep;

     CAffinity(void);
     ~CAffinity();
     REWARDITEM * GetNextReward(int);
     REWARDSTEP * FindRewardInfo(int);
     int AddPoint(int, int /*CPC **/);
     void UpdateRewardStep(int, int);
 };
typedef std::map<int, CAffinity*, std::less<int>, std::allocator<std::pair<int const, CAffinity*> > > map_affinitylist_t;
class CAffinityList {
       public:
         map_affinitylist_t m_list;

         CAffinityList(void);
         ~CAffinityList();
         bool AddAffinity(int, int);
         bool UpdateRewardPoint(int, int);
         CAffinity * FindAffinity(int);
         map_affinitylist_t * GetList(void);
         void SendRewardNotice(CPC *);
     };

class CAffinityProtoList {
   public:
     int m_AffinityTotal;
     CAffinityProto *m_affinity;

     CAffinityProtoList(void);
     ~CAffinityProtoList();
     CAffinityProto * FindProtoByAffinityIdx(int);
     CAffinityProto * FindProtoByNPCIdx(int);
     bool LoadAffinity(void);
     static int CompByAffinityIdx(const void *, const void *);
 };

#endif