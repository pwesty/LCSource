#pragma once

#include "MessageType.h"

class CExpedMember {
   private:
     int m_nCharIndex;
     CLCString m_strCharName;
     int m_nGroupType;
     int m_nMemberType;
     int m_nLabelType;
     int m_nListIndex;
     int m_nQuitType;
     CPC *m_pChar;
   public:
     int m_nLevel;

     CExpedMember(int, const char *, int, int, int, CPC *);
     ~CExpedMember();
     int GetCharIndex(void) const;
     const char * GetCharName(void) const;
     int GetGroupType(void) const;
     int GetMemberType(void) const;
     int GetLabelType(void) const;
     int GetQuitType(void) const;
     int GetListIndex(void) const;
     void SetGroupType(int);
     void SetListIndex(int);
     void SetMemberType(int);
     void SetLabelType(int);
     void SetQuitType(int);
     CPC * GetMemberPCPtr(void) const;
     void SetMemberPCPtr(CPC *);
 };

class CExpedition {
   private:
     char m_nExpedType[5];
     int m_nTotCount;
     int m_nGTotCount[4];
     CExpedMember *m_listMember[4][8];
     int m_nRequestIndex;
     CLCString m_strRequestName;
     int m_nEndExpedTime;
   public:
     int m_nRaidZoneNum;
     int m_nRaidAreaNum;
     int m_nSubNumber;
     int m_nDifficulty;

     CExpedition(char, char, char, char, char, int, const char *, int, int, CPC *);
     ~CExpedition();
     int GetRequestIndex(void) const;
     const char * GetRequestName(void) const;
     int GetMemberCount(void) const;
     char GetExpedType(int);
     int GetMemberCountOnline(void);
     int GetGroupByListindex(int);
     int GetBossIndex(void);
     const char * GetBossName(void);
     int GetBossGroupType(void);
     int FindMemberListIndex(int);
     int FindMemberGroupIndex(int);
     int GetMemberCharIndex(int, int);
     static int CompExpedition(CExpedition *, CExpedition *);
     void SetRequest(int, const char *);
     CExpedMember * JoinRequest(const char *, int, CPC *);
     CExpedMember * Join(int, const char *, int, CPC *);
     void DeleteMember(int);
     CExpedMember * GetMemberByCharIndex(int) const;
     CExpedMember * GetMemberByListIndex(int, int) const;
     CExpedMember * GetMemberByListIndex(int, int, int) const;
     bool ChangeBoss(int, int);
     void SetExpedType(int, char);
     int GetGroupMemberCount(int);
     int GetGroupBeEmptySlot(void);
     int GetGroupMembertypeCount(int, int);
     bool SetMBoss(int);
     bool ResetMBoss(int);
     bool MoveGroup(int, int, int, int);
     const CExpedMember * FindNextBoss(void);
     void SetEndExped(void);
     int GetNearExpeditionMemberCount(CCharacter *);
     CPC * GetNearMember(CCharacter *, int, int);
     void SetMemberPCPtr(int, CPC *);
     void SendToAllPC(CNetMsg &, int);
     void SendToPCInSameZone(int, int, CNetMsg &);
     void SendToPCInSameGroup(int, CNetMsg &);
     void SetEndExpedTime(int);
     int GetEndExpedTime(void);
     int GetOnlineMemberCount(void);
     bool SetMemberRegister_AfterGoZone(CPC *);
     CPC * GetMemberPCPtr(int, int);
 };