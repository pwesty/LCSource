#ifndef __GUILD_H__
#define __GUILD_H__

#include "MessageType.h"
#include "LCString.h"
#include "NetMsg.h"
#include "Skill.h"

class CPC;
class CGuild;

typedef struct __tagGuildSignal {
       MSG_GUILD_POSITION_TYPE flag;
       int senderIndex;
       float x;
       float z;
   } GUILD_SIGNAL; 

class CGuildKick {
   private:
     int m_masterKickStauts;
     int m_masterKickRequestChar;
     int m_masterKickRequestTime;

   public:
     CGuildKick(void);
     ~CGuildKick();
     void setKickStatus(int);
     int getKickStatus(void);
     void setKickReuestChar(int);
     int getKickRequestChar(void);
     void setKickRequestTime(int);
     int getKickRequestTime(void);
 };

class CGuildMember {
       private:
         int m_charindex;
         CLCString m_name;
         int m_pos;
         CGuild *m_guild;
         int m_listindex;
         char m_online;
         CPC *m_pc;
         int m_contributeExp;
         int m_contributeFame;
         int m_cumulatePoint;
         CLCString m_positionName;
         int m_channel;
         int m_zoneindex;

         CGuildMember(void);
       public:
         ~CGuildMember();
         int charindex(void);
         void charindex(int);
         const char * GetName(void);
         void name(const char *);
         MSG_GUILD_POSITION_TYPE pos(void);
         void pos(int);
         CGuild * guild(void);
         void guild(CGuild *);
         int listindex(void);
       private:
         void listindex(int);
       public:
         char online(void);
         void online(char);
         char GetGuildMark(void);
         void contributeExp(int);
         void contributeFame(int);
         void cumulatePoint(int);
         void AddCumulatePoint(int);
         void positionName(CLCString &);
         void channel(int);
         void zoneindex(int);
         int GetcontributeExp(void);
         int GetcontributeFame(void);
         int GetcumulatePoint(void);
         const char * GetPositionName(void);
         int GetChannel(void);
         int GetZoneNum(void);
         void SetPC(CPC *);
         CPC * GetPC(void);
     };

class CGuild {
   private:
     int m_index;
     CLCString m_name;
     int m_level;
     CGuildMember *m_boss;
     CGuildMember *m_officer[2];
     CGuildMember *m_member[200];
     int m_membercount;
     CGuild *m_next;
     CGuild *m_prev;
     int m_battleIndex;
     int m_battlePrize;
     int m_battleZone;
     int m_battleTime;
     int m_killCount;
     char m_battleState;
     int m_battlePulse;
     GUILD_SIGNAL m_guildSignal;
     int m_landcount;
     int m_land[256];
     int m_GuildPoint;
     char m_incline;
     int m_maxmember;
     int m_GuildPointRanking;
   public:
     CSkillList m_passiveSkillList;
     CSkillList m_activeSkillList;
     CSkillList m_etcSkillList;
     bool m_bRebirthPos;
     bool m_bGmtarget;
     int m_cubeUniqueIdx;
     int m_nCountRushCaptain;
     int m_nCountSupportCaptain;
     int m_nCountReconCaptain;
     int m_nCountTotalGradeEx;
   private:
     char m_GuildMark_row;
     char m_GuildMark_col;
     char m_Background_row;
     char m_Background_col;
     int m_GuildMarkExpire;
     CGuildKick m_guildKick;

   public:
     CSkill * FindSkill(int);
     CGuild(int, const char *, int, int, int, int, int, int, int);
     ~CGuild();
     CPC * GetNearMember(CCharacter *, int);
     void landcount(int);
     void land(int, int *);
     void guildPoint(int);
     void incline(char);
     void AddGuildPoint(int);
     int GetLandCount(void);
     int * GetLand(void);
     int GetGuildPoint(void);
     char GetIncline(void);
     void guildPointRanking(int);
     int GetGuildPointRanking(void);
     bool IsRebrithPos(void);
     void SetRerithPos(bool);
     int index(void);
     void changeName(const char *);
     const char * name(void);
     int level(void);
     void level(int);
     void LevelUp(void);
     CGuildMember * boss(void);
     CGuildMember * officer(int);
     CGuild * nextguild(void);
     void nextguild(CGuild *);
     CGuild * prevguild(void);
     void prevguild(CGuild *);
     int membercount(void);
     CGuildMember * member(int);
     int battleIndex(void);
     int battlePrize(void);
     int battleZone(void);
     void BattleSet(int, int, int);
     int battlePulse(void);
     void BattlePulse(int);
     int battleTime(void);
     void BattleTime(int);
     int killCount(void);
     void KillCount(int);
     char battleState(void);
     void BattleState(char);
     GUILD_SIGNAL * guildSignal(void);
     void guildSignal(_tagGuildPositionType, int, float, float);
     int maxmember(void);
     void setMaxmember(int);
     int addmember(int, const char *);
     void removemember(CGuildMember *);
     CGuildMember * changeboss(int);
     void appoint(int);
     void fire(int);
     CGuildMember * findmember(int);
     CGuildMember * findmember(const char *);
     void SendToAll(CNetMsg &, bool);
     void SendToAllInSameZone(CNetMsg &, int);
     void InitGradeExPosCount(void);
     bool CheckGradeExPosCount(int);
     void ChangeGradeExPosCount(int, int);
     void DelGradeExPosCount(int);
     void AddGradeExPosCount(int);
     int GetGradeExPosNeedGuilPoint(int);
     void SetGuildMark(char, char, char, char, int);
     char GetGuildMarkRow(void);
     char GetGuildMarkCol(void);
     char GetBackgroundRow(void);
     char GetBackgroundCol(void);
     int GetMarkTime(void);
     int GetMarkTimeConvertGameTime(void);
   private:
     int getStandardTimeForKick(void);
     int getBossDateStamp(void);
     int getLastChansgeBossTime(void);
   public:
     CGuildKick * getGuildKick(void);
     bool isPossibleKickMaster(void);
     bool isPastLastChangeBoss(void);
     int getNewBossByKick(void);
 };

 class CGuildList {
   private:
     CGuild *m_head;
     int m_count;

   public:
     CGuildList(void);
     ~CGuildList();
     static CGuild * create(int, const char *, int, const char *);
     static CGuild * create(int, const char *, int, int, int, int, int, int, int);
     void add(CGuild *);
     void Remove(CGuild *);
     CGuild * findguild(int);
     CGuild * findguild(const char *);
     CGuildMember * findmember(int);
     void Reset(void);
     CGuild * head(void);
 };

#endif