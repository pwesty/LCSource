#ifndef __EVENT_H_
#define __EVENT_H_

#include "LCString.h"
#include "LCList.h"
#include "MessageType.h"
#include "NetMsg.h"
#include <string>

struct COXQuizData {
       private:
         int m_nIndex;
         CLCString m_strQuiz;
         bool m_bAnswerOX;

       public:
         COXQuizData(int, const char *, bool);
         int GetQuizIndex(void) const;
         const char * GetQuizString(void) const;
         bool GetAnswer(void) const;
         static int CompOXData(COXQuizData *, COXQuizData *);
     };

struct CGoldenBallEventData {
       private:
         int m_nStatus;
         CLCString m_strTeam1;
         CLCString m_strTeam2;
         int m_nTeam1Score;
         int m_nTeam2Score;
         int m_nEndYear;
         int m_nEndMonth;
         int m_nEndDay;
         int m_nEndHour;
         int m_nEndMinute;
         time_t m_tEndTime;
         time_t m_tVoteEndTime;

       public:
         CGoldenBallEventData(void);
         int GetStatus(void);
         const char * GetTeam1Name(void);
         const char * GetTeam2Name(void);
         int GetTeam1Score(void);
         int GetTeam2Score(void);
         int GetEndYear(void);
         int GetEndMonth(void);
         int GetEndDay(void);
         int GetEndHour(void);
         int GetEndMinute(void);
         time_t GetEndTime(void);
         time_t GetVoteEndTime(void);
         void Vote(const char *, const char *, int, int, int, int, int);
         void Gift(const char *, int, const char *, int, int, int, int, int, int, long);
         void EndVote(void);
         void End(void);
     };

struct CGrobalEcho {
   private:
     unsigned int remainTime[10];
     unsigned int repeatTime[10];
     unsigned int checkClock[10];
     CNetMsg msg[10];

     int FindIndex(void);
   public:
     CGrobalEcho(void);
     void Set(CNetMsg &, unsigned int, unsigned int);
     void Print(void);
 };


class CMoonStoneRewardData {
	struct __reward {
     int rewardindex;
     int rewardcount;
     int rewardflag;
     float prob;
 };
     int m_nMoonStoneIndex;
     int m_nTotalProb;
     CLCList<CMoonStoneRewardData::__reward*> m_listReward;
   public:
     CMoonStoneRewardData(void);
     ~CMoonStoneRewardData();
     static int CompRewardByIndex(CMoonStoneRewardData::__reward *, CMoonStoneRewardData::__reward *);
     bool Load(int);
 };

class CMoonStoneReward {
     int m_nRewardDataCount;
     CMoonStoneRewardData *m_pReward;
   public:
     CMoonStoneReward(void);
     ~CMoonStoneReward();
     bool Load(void);
     static int Comp(const void *, const void *);
     CMoonStoneRewardData * Find(int);
 };

typedef struct _giveItemInfo {
       int index;
       LONGLONG count;
       int flag;
       int prob;
     public:
       _giveItemInfo(void);
   } GiveItemInfo; 

typedef struct _courseInfo {
       int index;
       int coinUseCnt;
       int *coinIndex;
       int *coinDefCnt;
       int *coinAddCnt;
       GiveItemInfo *giveItem;
     public:
       _courseInfo(void);
   } CourseInfo; 

class CLacarette {
   protected:
     int m_count;
     CourseInfo *m_data;

   public:
     CLacarette(void);
     ~CLacarette();
     bool Load(void);
     void RemoveAll(void);
     int GetNeedTokenCount(int, int, int, int &);
     CourseInfo * GetCourseData(int);
 };

struct CWGPlayer {
         private:
           int m_nWarType;
           int m_nCharIndex;
           int m_nLevelType;
           int m_nSelected;
           CWGPlayer *m_next;

         public:
           CWGPlayer(void);
           CWGPlayer(int, int, int, int);
           ~CWGPlayer();
           void SetCharIndex(int);
           void SetLevelType(int);
           void SetSelected(int);
           void SetNext(CWGPlayer *);
           void SetWarType(int);
           int GetCharIndex(void);
           int GetLevelType(void);
           int GetSelected(void);
           CWGPlayer * GetNext(void);
           int GetWarType(void);
       };

class CWGPlayerList {
       private:
         CWGPlayer *m_head;
         int m_nTotalCount;
         int m_nRookieCount;
         int m_nSeniorCount;
         int m_nMasterCount;

       public:
         CWGPlayerList(void);
         ~CWGPlayerList();
         void AddTotalCount(int);
         void AddRookieCount(int);
         void AddSeniorCount(int);
         void AddMasterCount(int);
         int GetTotalCount(void);
         int GetRookieCount(void);
         int GetSeniorCount(void);
         int GetMasterCount(void);
         void AddNode(CWGPlayer *);
         void DelNode(int);
         bool FindNode(int);
         CWGPlayer * GetNode(int);
         CWGPlayer * GetHead(void);
         bool Select(void);
         void ClearList(void);
         void SendResult(void);
         void SetHead(CWGPlayer *);
         CWGPlayer * GetTail(void);
         void ShuffleWGPlayer(int);
     };

class CWaitPlayer {
         private:
           bool m_bCheckIn;
           int m_nWGType;
           int m_nLevelType;
           char m_cCharType;
           int m_nCharIndex;
           bool m_bIsEntered;
           int m_nTeam;
           CWaitPlayer *m_next;
           bool m_bHold;

         public:
           CWaitPlayer(void);
           CWaitPlayer(int, int, char, int, bool, int);
           ~CWaitPlayer();
           bool GetCheckIn(void);
           int GetWGType(void);
           int GetLevelType(void);
           char GetCharType(void);
           int GetCharIndex(void);
           bool GetIsEntered(void);
           int GetTeam(void);
           CWaitPlayer * GetNext(void);
           void SetCheckIn(bool);
           void SetWGType(int);
           void SetLevelType(int);
           void SetCharType(char);
           void SetCharIndex(int);
           void SetEntered(bool);
           void SetTeam(int);
           void SetNext(CWaitPlayer *);
           void SetHold(bool);
           bool GetHold(void);
           void UnHold(void);
       };

class CWaitPlayerList {
       private:
         CWaitPlayer *m_head;
         int m_nPlayerCount;
         int m_nRookieCount;
         int m_nSeniorCount;
         int m_nMasterCount;

       public:
         CWaitPlayerList(void);
         ~CWaitPlayerList();
         int GetPlayerCount(void);
         void AddNode(CWaitPlayer *);
         CWaitPlayer * GetNode(int);
         void DelNode(int);
         bool FindNode(int);
         CWaitPlayer * GetHead(void);
         CWaitPlayer * GetTail(void);
         void SetHead(CWaitPlayer *);
         void ClearList(void);
         void AddRookieCount(int);
         void AddSeniorCount(int);
         void AddMasterCount(int);
         int GetRookieCount(void);
         int GetSeniorCount(void);
         int GetMasterCount(void);
         void SendAllWaitPlayer(CNetMsg &);
         void ShuffleWaitPlayer(int);
     };

class CRoyalRumble {
   private:
     time_t m_tRoyalRumbleTime;
     time_t m_tRookieTime;
     time_t m_tSeniorTime;
     time_t m_tMasterTime;
     int m_nRoyalRumbleProcess;
     bool m_bRoyalRumbleNotice;
     bool m_bIsRookiePlaying;
     bool m_bIsSeniorPlaying;
     bool m_bIsMasterPlaying;
     bool m_bRookieLastEnter;
     bool m_bSeniorLastEnter;
     bool m_bMasterLastEnter;
     int m_nRookieWinnerCharIndex;
     int m_nSeniorWinnerCharIndex;
     int m_nMasterWinnerCharIndex;
     int m_nWinnerIsRewarded;
   public:
     bool m_bCondition;
     CWGPlayerList m_WGPlayerList;
     CWaitPlayerList m_WaitPlayerList;
     int m_RoyalRumbleTime[5];
     CZone *m_pRoyalRumbleZone;
     CArea *m_pRookieArea;
     CArea *m_pSeniorArea;
     CArea *m_pMasterArea;

     CRoyalRumble(void);
     ~CRoyalRumble();
     void Init(void);
     void CheckRoyalRumble(void);
     void CheckRoyalRumbleArea(void);
     void SetRoyalRumbleTime(long);
     void AddRoyalRumbleTime(long);
     time_t GetRoyalRumbleTime(void);
     void SetRoyalRumbleNextProcess(void);
     void SetInitRoyalRumbleProcess(void);
     int GetRoyalRumbleProcess(void);
     void SetRoyalRumbleNotice(bool);
     bool GetRoyalRumbleNotice(void);
     void SetRookiePlaying(bool);
     void SetSeniorPlaying(bool);
     void SetMasterPlaying(bool);
     bool GetRookiePlaying(void);
     bool GetSeniorPlaying(void);
     bool GetMasterPlaying(void);
     void MakeWaitPlayer(void);
     void SetInitRookieTime(void);
     void SetInitSeniorTime(void);
     void SetInitMasterTime(void);
     void AddRookieTime(long);
     void AddSeniorTime(long);
     void AddMasterTime(long);
     time_t GetRookieTime(void);
     time_t GetSeniorTime(void);
     time_t GetMasterTime(void);
     void SetRookieLastEnter(bool);
     void SetSeniorLastEnter(bool);
     void SetMasterLastEnter(bool);
     bool GetRookieLastEnter(void);
     bool GetSeniorLastEnter(void);
     bool GetMasterLastEnter(void);
     void EnterTheWarGround(int);
     int GetRemainPlayer(int);
     int GetEnteredPlayer(int);
     void SetDebuff(int, int);
     void UnHold(void);
     int GetWinner(int);
     void SetWinner(int, CPC *);
     void SetRewarded(int, CPC *);
     bool GetRewarded(int);
     void InitRewarded(int);
     int GetRemainWaitTime(void);
 };

struct CAttachItemData {
   private:
     int index;
     LONGLONG count;
     int slot;

   public:
     CAttachItemData(void);
     CAttachItemData(int, long long, int);
     CAttachItemData & operator=(const CAttachItemData &);
     void setIndex(int);
     int getIndex(void);
     void setCount(long long);
     LONGLONG getCount(void);
     void setSlot(int);
     int getSlot(void);
 };

struct CPostOfficeAgent {

   public:
     CPostOfficeAgent(void);
     ~CPostOfficeAgent();
     int AuthUser(int);
     int find(char *);
     char * getName(int);
     int getIdx(char *);
     int setAddInven(int, int, int, int, int, int, long long, short, long long);
     int addItemUserInven(CPC *, int, int, int, int, long long, short, long long);
     int addMoneyUserInven(CPC *, int, int, int, int, long long);
     int checkUserMailListReq(int, int);
     int setDeleteInven(void);
     int sendMail(CNetMsg &);
     int checkUserMailBox(int);
     bool checkUserMoney(CPC *, long long);
     void transmitUserMoney(CPC *, long long);
     void giveUserMoney(CPC *, long long);
     void itemClean(std::vector<CAttachItemData*, std::allocator<CAttachItemData*> > &);
     void checkItemFlagOption(CPC *, CItem *, int &, int &, short *, char *);
     int checkNpcDistance(CPC *, int);
     int getUserNameToIndex(CLCString);
     bool sendItems(CPC *, const std::vector<CAttachItemData*, std::allocator<CAttachItemData*> > &);
   private:
     bool checkExistItems(CPC *, const std::vector<CAttachItemData*, std::allocator<CAttachItemData*> > &, std::vector<CItem*, std::allocator<CItem*> > &);
     bool removeItems(CPC *, const std::vector<CAttachItemData*, std::allocator<CAttachItemData*> > &);

     typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > string;

	 private:
     static string SQL_INSERT_POSTOFFICE;
     static string SQL_INSERT_POSTOFFICEATTACHMENT;
     static string SQL_SELECT_USERATTACHMENT;
     static string SQL_DELETE_USERATTACHMENT;
     static string SQL_SELECT_USERMAILBOX;
     static string SQL_UPDATE_USERMAILBOXMONEY;
 };

struct CRewardData {
   protected:
     int m_jobFlag;
     int m_levelMini;
     int m_levelMaxi;
     int m_prob;
     int m_rewardType;
     int m_index;
     int m_value1;
     int m_value2;
     LONGLONG m_value3;

   public:
     CRewardData(int, int, int, int, int, int, int, int, long long);
     ~CRewardData();
     const int getJobFlag(void);
     const int getLevelMini(void);
     const int getLevelMaxi(void);
     const int getProb(void);
     const int getRewardType(void);
     const int getIndex(void);
     const int getValue1(void);
     const int getValue2(void);
     const LONGLONG getValue3(void);
     const int getItemIndex(void);
     const int getItemPlus(void);
     const int getItemFlag(void);
     const LONGLONG getItemCount(void);
     const LONGLONG getNasCount(void);
     const LONGLONG getExp(void);
     const LONGLONG getSp(void);
     const int getStat(void);
     const int getSkillIndex(void);
     const int getSkillLevel(void);
     int checkJob(int);
 };
typedef struct __reward_data_info {
       int type;
       int index;
       int value1;
       int value2;
       LONGLONG value3;
     public:
       __reward_data_info(void);
   } REWAD_DATA_INFO; 

struct CReward {
   private:
     int m_rewardIndex;
     int m_probType;
     std::vector<CRewardData, std::allocator<CRewardData> > m_rewardDataList;

   public:
     CReward(int, int);
     ~CReward();
     void pushData(CRewardData);
     const int getRewardCount(void);
     CRewardData * getRewardData(int);
     const int getProbType(void);
     const int getRewardIndex(void);
 };

class CRewardManager {
   private:
     std::map<int, CReward, std::less<int>, std::allocator<std::pair<int const, CReward> > > m_dataList;
     std::vector<__reward_data_info, std::allocator<__reward_data_info> > m_rewardDataList;
     int m_invenSlot;
     int m_invenWeight;

   public:
     CRewardManager(void);
     ~CRewardManager();
     bool loadReward(MYSQL *);
     bool setRewardData(CPC *, int, int, int);
     bool doReward(CPC *, int);
     const std::vector<__reward_data_info, std::allocator<__reward_data_info> > * getRewardDataList(void);
     int getRewardDataCount(void);
     int getRewardDataType(int);
     int getRewardDataIndex(int);
     int getRewardDataValue1(int);
     int getRewardDataValue2(int);
     int getRewardDataValue3(int);
   protected:
     bool setRewardDataMulti(CPC *, CReward *, int);
     bool setRewardDataOnceRandom(CPC *, CReward *, int);
     bool setRewardDataOnceSelect(CPC *, CReward *, int);
     void initRewardDataInfo(void);
     bool checkProto(CRewardData *);
     bool checkRewardCondition(CPC *, CRewardData *, int, bool);
     bool pushRewardData(CRewardData *);
     bool pushRewardDataItem(CRewardData *);
     bool reward2Pc(CPC *);
     bool item2PC(CPC *, REWAD_DATA_INFO *);
     bool nas2PC(CPC *, REWAD_DATA_INFO *);
     bool exp2PC(CPC *, REWAD_DATA_INFO *);
     bool sp2PC(CPC *, REWAD_DATA_INFO *);
     bool stat2PC(CPC *, REWAD_DATA_INFO *);
     bool skill2PC(CPC *, REWAD_DATA_INFO *);
 };

class CDropItemData {
   private:
     int m_index;
     int m_plus;
     int m_flag;
     LONGLONG m_count;
     int m_prob;
     int m_jobFlag;

   public:
     CDropItemData(int, int, int, long long, int, int);
     ~CDropItemData();
     const int getIndex(void);
     const int getPlus(void);
     const int getFlag(void);
     const LONGLONG getCount(void);
     const int getProb(void);
     const int getJobFlag(void);
 };

class CProbByLevelSection {
   private:
     int m_start;
     int m_end;
     int m_prob;

   public:
     CProbByLevelSection(int, int, int);
     ~CProbByLevelSection();
     const int getStart(void);
     const int getEnd(void);
     const int getProb(void);
 };

class CDropInfo {
   protected:
     int m_index;
     int m_dropType;
     int m_levelMini;
     int m_levelMaxi;
     int m_prob;
     int m_probPerLevel;
     int m_npcTypeParty;
     int m_npcTypeBoss;
     std::vector<int, std::allocator<int> > m_npcList;
     std::vector<CDropItemData, std::allocator<CDropItemData> > m_itemList;
     std::vector<CProbByLevelSection, std::allocator<CProbByLevelSection> > m_levelSectionList;

   public:
     CDropInfo(int, int, int, int, int, int, int, int);
     ~CDropInfo();
     const int getNpcCount(void);
     void addNpc(int);
     const int getNpcIndex(int);
     const int getItemCount(void);
     void addItem(CDropItemData);
     CDropItemData * getItemData(int);
     const int getLevelSectionCount(void);
     void addLevelSection(CProbByLevelSection);
     CProbByLevelSection * getLevelSection(int);
     const int getIndex(void);
     const int getDropType(void);
     const int getLevelMini(void);
     const int getLevelMaxi(void);
     const int getProb(void);
     const int getProbPerLevel(void);
     const int getNpcTypeParty(void);
     const int getNpcTypeBoss(void);
 };

class CDropManager {
   private:
     std::map<int, CDropInfo, std::less<int>, std::allocator<std::pair<int const, CDropInfo> > > m_dataList;

   public:
     CDropManager(void);
     ~CDropManager();
     bool loadDropData(MYSQL *);
     bool doDrop(CPC *, CNPC *, int, int, int, bool);
     int getDropItemListCount(int);
   private:
     CDropInfo * getDropInfo(int);
     bool checkCondition(CPC *, CNPC *, CDropInfo *);
     bool checkNpc(CDropInfo *, int);
     bool checkLevelLimit(CDropInfo *, int, int);
     bool checkDropProb(CNPC *, CDropInfo *);
     int getDropProb(CNPC *, CDropInfo *);
     int getProbByLevelSection(CDropInfo *, int);
     bool doDropRandom(CPC *, CNPC *, CDropInfo *, bool);
     bool doDropOnceRandom(CPC *, CNPC *, CDropInfo *, int, bool);
     bool doDropOnceSelect(CPC *, CNPC *, CDropInfo *, int, bool);
     bool checkProbPerLevel(int, int, int);
     int getProbPerLevel(int, int);
     bool checkJob(int, int);
     bool dropItem(CPC *, CNPC *, CDropItemData *, bool);
     int getRandom(void);
 };

class CEventInfo {
   private:
     int m_index;
     bool m_enable;
     bool m_notice;
     std::vector<int, std::allocator<int> > m_rewardList;
     std::vector<int, std::allocator<int> > m_npcList;
     std::vector<int, std::allocator<int> > m_dropList;
     std::vector<CNPCRegenInfo*, std::allocator<CNPCRegenInfo*> > m_npcRegenList;

   public:
     CEventInfo(void);
     ~CEventInfo();
     void setEventIndex(int);
     void setEnable(bool);
     void setNotice(bool);
     void addRewardIndex(int);
     const int getEventIndex(void);
     const bool getEnable(void);
     const bool getNotice(void);
     const int getRewardIndexCount(void);
     const int getRewardIndex(int);
     void addNpc(int);
     int getNpcCount(void);
     bool isEventNpc(int);
     int getNpcDBIndex(int);
     void addDrop(int);
     int getDropCount(void);
     int getDropIndex(int);
     void addNpcRegenInfo(CNPCRegenInfo *);
     void setRegenDisable(void);
     void setRegenEnable(void);
   private:
     void setRegen(bool);
 };

class CEventAutomationManager {
   private:
     std::map<int, CEventInfo, std::less<int>, std::allocator<std::pair<int const, CEventInfo> > > m_eventList;
     std::map<int, CEventInfo*, std::less<int>, std::allocator<std::pair<int const, CEventInfo*> > > m_activeList;

   public:
     CEventAutomationManager(void);
     ~CEventAutomationManager();
     bool loadEvent(void);
     CEventInfo * getEventInfo(int);
     CEventInfo * getActiveEventInfo(int);
     bool addActiveEvent(int, bool);
     bool removeActiveEvent(int, bool);
     bool isActiveEvent(int);
     bool isEventNpc(int, int &, bool &);
     bool isActiveEventNpc(int, int &);
     void addNpcRegenInfo(CNPCRegenInfo *);
     std::map<int, CEventInfo*, std::less<int>, std::allocator<std::pair<int const, CEventInfo*> > > * getActiveEvenList(void);
     std::map<int, CEventInfo, std::less<int>, std::allocator<std::pair<int const, CEventInfo> > > * getEvenList(void);
     int getActiveEventCount(void);
   private:
     bool dbUpdate(int, bool);
 };
#endif