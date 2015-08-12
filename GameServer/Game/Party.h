#ifndef __PARTY_H__
#define __PARTY_H__

#define		PARTY_TYPE_RANDOM		0		// 균등 파티
#define		PARTY_TYPE_FIRSTGET		1		// 입수 우선
#define		PARTY_TYPE_BATTLE		2		// 전투형
#define		PARTY_TYPE_OPENBOX		3		// 상자열기

#define		MAX_PARTY_DIVITYPE		3		// 분배방법 수

class CNetMsg;
class CMonsterCombo;


#define PARTY_MATCHING_COMMENT			50		// 파티 매칭 파티 등록시 설명문구 길이
#define PARTY_MATCHING_DATA_PER_PAGE	10		// 파티 매칭 리스트에서 페이지당 표시할 데이터 수
#define PARTY_MATCHING_LIMIT_LEVEL		2		// 파티 매칭에서 레벨 제한 범위

class CPartyMatchMember {
   private:
     int m_nIndex;
     CLCString m_strName;
     int m_nLevel;
     int m_nZone;
     char m_nJob;
     char m_nPartyType;

   public:
     CPartyMatchMember(int, const char *, int, int, char, char);
     int GetIndex(void) const;
     const char * GetName(void) const;
     int GetLevel(void) const;
     int GetZone(void) const;
     char GetJob(void) const;
     char GetPartyType(void) const;
     void SetName(const char *);
     void SetLevel(int);
     void SetZone(int);
     static int CompByIndex(CPartyMatchMember *, CPartyMatchMember *);
 };

class CPartyMatchParty {
   private:
     int m_nBossIndex;
     CLCString m_strBossName;
     int m_nBossLevel;
     int m_nZone;
     int m_nJobFlag;
     char m_nPartyType;
     bool m_bLimitLevel;
     CLCString m_strComment;
     int m_nMemberCnt;

   public:
     CPartyMatchParty(int, const char *, int, int, int, char, bool, const char *);
     int GetBossIndex(void) const;
     const char * GetBossName(void) const;
     int GetBossLevel(void) const;
     int GetZone(void) const;
     int GetJobFlag(void) const;
     char GetPartyType(void) const;
     bool IsLimitLevel(void) const;
     const char * GetComment(void) const;
     void SetBossIndex(int);
     void SetBossName(const char *);
     void SetBossLevel(int);
     void SetZone(int);
     int GetMemberCount(void) const;
     void SetMemberCount(int);
     static int CompByBossIndex(CPartyMatchParty *, CPartyMatchParty *);
 };

// 파티 멤버
class CPartyMember {
       private:
         int m_nCharIndex;
         CLCString m_strCharName;
         CPC *m_pChar;
       public:
         int m_nLevel;

         CPartyMember(int, const char *, CPC *);
         ~CPartyMember();
         int GetCharIndex(void) const;
         const char * GetCharName(void) const;
         CPC * GetMemberPCPtr(void) const;
         void SetMemberPCPtr(CPC *);
     };

class CParty {
     char m_nPartyType[5];
     int m_nCount;
     CPartyMember *m_listMember[8];
     int m_nRequest;
     CLCString m_strRequest;
     int m_nEndPartyTime;
     int m_nRaidZoneNum;
     int m_nRaidAreaNum;
     int m_nSubNumber;
     bool m_bGmtarget;
     int m_comboAreaIndex;
     int m_cubeUniqueIdx;
   public:
     CParty(char, int, const char *, CPC *, int, const char *);
     ~CParty();
     int GetRequestIndex(void) const;
     const char * GetRequestName(void) const;
     int GetMemberCount(void) const;
     char GetPartyType(int) const;
     int GetMemberCountOnline(void);
     int GetBossIndex(void) const;
     int FindMember(int);
     int GetMemberCharIndex(int);
     static int CompParty(CParty *, CParty *);
     void SetRequest(int, const char *);
     int JoinRequest(const char *, CPC *);
     void DeleteMember(int);
     CPartyMember * GetMemberByListIndex(int) const;
     bool ChangeBoss(const char *);
     void SetPartyType(int, char);
     void SetPartyTypeAll(char);
     void SetEndParty(void);
     int GetNearPartyMemberCount(CCharacter *);
     CPC * GetNearMember(CCharacter *, int);
     void SetMemberPCPtr(int, CPC *);
     void SendToAllPC(CNetMsg &, int);
     void SendToSameContinentPC(CNetMsg &, int);
     void SendToPartyRecallPC(CNetMsg &, int);
     void SendToPCInSameZone(int, int, CNetMsg &);
     void SetEndPartyTime(int);
     int GetEndPartyTime(void);
     int GetOnlineMemberCount(void);
     bool SetMemberRegister_AfterGoZone(CPC *);
 };

#endif