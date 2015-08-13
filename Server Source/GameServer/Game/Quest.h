// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
#define AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Character.h"
class CNPC;
class CQuest;

// 퀘스트 프로토 타입
class CQuestProto {
public:
     int m_index;
     int m_type[2];
     int m_startType;
     int m_startData;
     int m_prizeNPC;
     int m_needMinLevel;
     int m_needMaxLevel;
     int m_needJob;
     int m_needItemIndex[5];
     int m_needItemCount[5];
     int m_prequestNum;
     int m_startNpcZoneNum;
     int m_prizeNpcZoneNum;
     int m_needExp;
     int m_conditionType[3];
     int m_conditionIndex[3];
     int m_conditionNum[3];
     int m_conditionData[3][4];
     int m_prizeType[5];
     int m_prizeIndex[5];
     LONGLONG m_prizeData[5];
     int m_optPrizeType[7];
     int m_optPrizeIndex[7];
     int m_optPrizeData[7];
     int m_failValue;
     int m_partyScale;
     CLCList<CAffinityProto*> m_affinityList;
     int m_StartGiveItem[10];
     int m_StartGiveKindCount;
     int m_StartGiveNumCount[10];
   public:
     CQuestProto(void);
 };

class CQuestProtoList
{
public:
	CQuestProto*	m_proto;		// 퀘스트 프로토 리스트
	int				m_nCount;		// 퀘스트 수

	CQuestProtoList();
	~CQuestProtoList();

	bool Load();							// DB 읽기
	CQuestProto* FindProto(int index);		// 퀘스트 proto 찾기

#ifdef QUEST_DATA_EXTEND
#else
	CQuest* Create(int index);	// Quest 생성
#endif // QUEST_DATA_EXTEND

	protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CQuestProto* i1 = (CQuestProto*)p1;
		CQuestProto* i2 = (CQuestProto*)p2;

		return i1->m_index - i2->m_index;
	}
};


class CQuest {
       private:
         const CQuestProto *m_pQuestProto;
         char m_cQuestState;
         int m_nQuestValue[3];
         bool m_bComplete1;
         bool m_bComplete2;
         CQuest *m_pPrev;
         CQuest *m_pNext;
         int m_nFailValue;
         long m_nCompleteTime;

       public:
         CQuest(const CQuestProto *);
         ~CQuest();
		void SetPrevQuest(CQuest* pPrev)		{ m_pPrev = pPrev; }
		void SetNextQuest(CQuest* pNext)		{ m_pNext = pNext; }
		CQuest* GetPrevQuest()					{ return m_pPrev; }
		CQuest* GetNextQuest()					{ return m_pNext; }

		char GetQuestState()					{ return m_cQuestState; }

		const CQuestProto* GetQuestProto()		{ return m_pQuestProto; }
		int GetQuestIndex()						{ return (m_pQuestProto) ? m_pQuestProto->m_index : -1; }
		int GetQuestType0()						{ return (m_pQuestProto) ? m_pQuestProto->m_type[0] : -1; }
		int GetQuestType1()						{ return (m_pQuestProto) ? m_pQuestProto->m_type[1] : -1; }
		int GetPrizeNPCIndex()					{ return (m_pQuestProto) ? m_pQuestProto->m_prizeNPC : -1; }
		int GetPrizeNPCZone()					{ return (m_pQuestProto) ? m_pQuestProto->m_prizeNpcZoneNum : -1; }
         bool IsTutorialQuest(void);
         bool IsPetQuest(void);
         void SetComplete1(bool);
         void SetComplete2(bool);
         bool GetComplete1(void);
         bool GetComplete2(void);
         bool IsCompleted(void);
         void IncreaseQuestValue(int, int);
         void SetQuestValue(int, int);
         int GetQuestValue(int);
         void QuestUpdateData(CPC *, CNPC *);
         void RemoeQuestItem(CPC *);
         void QuestUpdateDataForParty(CPC *, CNPC *);
         void QuestUpdateDataForParty(CPC *, CNPC *, int);
         int GetQuestType2(void);
         int GetPartyScale(void);
         bool CheckComplete(CPC *);
         void SetFailValue(int);
         int GetFailValue(void);
         void SetCompleteTime(long);
         long GetCompleteTime(void);
       private:
         void SetQuestState(char);
     };

/////////////////////
// 캐릭터 Quest List
class CQuestList {
   private:
     CQuest *m_head;
     int m_nCountRun;
     int m_nCountDone;
     int m_nCountAbandon;
     int m_nCountComplete;

   public:
     CQuestList(void);
     ~CQuestList();
     int GetCountRun(void);
     int GetCountDone(void);
     int GetCountAbandon(void);
     void DecreaseQuestComepleteCount(void);
     void IncreaseQuestComepleteCount(void);
     int GetQuestComepleteCount(void);
     void ResetQuestComepleteCount(void);
     CQuest * AddQuest(int);
     bool DelQuest(CPC *, CQuest *, char);
     CQuest * FindQuest(int);
     bool FindQuest(int, char);
     CQuest * FindQuestByItem(int);
     CQuest * FindQuestByMob(int);
     bool IsQuestType0(int);
     void SetQuestState(CQuest *, char);
     CQuest * GetNextQuest(CQuest *, char);
     void CheckComplete(CPC *);
};

#endif // !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
