// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
#define AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_

#include <vector>
#include "../ShareLib/MemoryPoolBase.h"

class CQuest;

class CAffinityProto;

typedef std::vector<CAffinityProto*>	vec_quest_affinityList_t;

// 퀘스트 프로토 타입
class CQuestProto
{
public:

	// DB
	int			m_index;							// 퀘스트 DB 번호
	int			m_type[2];							// 퀘스트 종류 : 방식/횟수에 따른 분류

	int			m_startType;						// 시작 방법
	int			m_startData;						// 시작 방법에 따른 데이타
	int			m_prizeNPC;							// 퀘스트를 완료시키는 NPC

	// 필요조건 : 퀘스트 성립 조건
	int			m_needMinLevel;							// 최소레벨
	int			m_needMaxLevel;							// 최대레벨
	int			m_needJob;								// 직업
	int			m_needItemIndex[QUEST_MAX_NEED_ITEM];	// 필요아이템 인덱스 : 없으면 -1
	int			m_needItemCount[QUEST_MAX_NEED_ITEM];	// 필요아이템 수량

	int			m_needRvRType;
	int			m_needRvRGrade;

	int			m_prequestNum;							//선행퀘스트 번호
	int			m_startNpcZoneNum;						//시작 NPC 존번호
	int			m_prizeNpcZoneNum;						//보상 NPC 존번호
	int		m_needExp;									//필요 경험치

	// 수행 조건 : 퀘스트 수행 조건
	int			m_conditionType[QUEST_MAX_CONDITION];	// 수행 종류
	int			m_conditionIndex[QUEST_MAX_CONDITION];	// 수행 종류 인덱스
	int			m_conditionNum[QUEST_MAX_CONDITION];	// 수행 수
	int			m_conditionData[QUEST_MAX_CONDITION][QUEST_MAX_CONDITION_DATA];	// 수행 조건 부가 데이타

	// 보상 조건
	int			m_prizeType[QUEST_MAX_PRIZE];		// 보상 종류
	int			m_prizeIndex[QUEST_MAX_PRIZE];		// 보상 인덱스
	LONGLONG			m_prizeData[QUEST_MAX_PRIZE];		// 보상 데이타

	int			m_optPrizeType[QUEST_MAX_OPTPRIZE];		// 선택 보상 종류
	int			m_optPrizeIndex[QUEST_MAX_OPTPRIZE];		// 선택 보상 인덱스
	int			m_optPrizeData[QUEST_MAX_OPTPRIZE];		// 선택 보상 데이타
	int			m_failValue;						// 실패 값

	int			m_partyScale;

	vec_quest_affinityList_t m_affinityList;

	int			m_StartGiveItem[QUESTITEM_MAX_ARRAYCOUNT];		// 퀘스트 시작 시 주는 아이템
	int			m_StartGiveKindCount;							// 퀘스트 시작 시 주는 아이템 종류
	int			m_StartGiveNumCount[QUESTITEM_MAX_ARRAYCOUNT];	// 퀘스트 시작 시 주는 아이템 획득 개수
	CQuestProto();
};

class CQuestProtoList
{
public:
	typedef std::map<int, CQuestProto*> map_t;

	map_t			map_;
	CQuestProto*	m_proto;		// 퀘스트 프로토 리스트
	int				m_nCount;		// 퀘스트 수

	CQuestProtoList();
	~CQuestProtoList();

	bool Load();							// DB 읽기
	CQuestProto* FindProto(int index);		// 퀘스트 proto 찾기
	CQuestProto* GetProtoIndexByNPC(int mobIndex );	// 몹인덱스 proto 찾기

private:
	std::vector<std::string> a_need_item_str;
	std::vector<std::string> a_need_item_count_str;

	std::vector<std::string> a_condition_type_str;
	std::vector<std::string> a_condition_index_str;
	std::vector<std::string> a_condition_num_str;
	std::vector<std::string> a_condition_data_str[QUEST_MAX_CONDITION];

	std::vector<std::string> a_prize_type_str;
	std::vector<std::string> a_prize_index_str;
	std::vector<std::string> a_prize_data_str;

	std::vector<std::string> a_opt_prize_type_str;
	std::vector<std::string> a_opt_prize_index_str;
	std::vector<std::string> a_opt_prize_data_str;
};

class CQuest : public MemoryPoolBaseWithMutex<CQuest>
{
	friend class CQuestList;

	const CQuestProto*	m_pQuestProto;
	char				m_cQuestState;
	int					m_nQuestValue[QUEST_MAX_CONDITION];
	bool				m_bComplete1;
	bool				m_bComplete2;

	CQuest*				m_pPrev;
	CQuest*				m_pNext;

	int					m_nFailValue;
	long				m_nCompleteTime;

public:
	CQuest(const CQuestProto* pQuestProto);
	~CQuest();

	void SetPrevQuest(CQuest* pPrev)
	{
		m_pPrev = pPrev;
	}
	void SetNextQuest(CQuest* pNext)
	{
		m_pNext = pNext;
	}
	CQuest* GetPrevQuest()
	{
		return m_pPrev;
	}
	CQuest* GetNextQuest()
	{
		return m_pNext;
	}

	char GetQuestState()
	{
		return m_cQuestState;
	}

	const CQuestProto* GetQuestProto()
	{
		return m_pQuestProto;
	}
	int GetQuestIndex()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_index : -1;
	}
	int GetQuestType0()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_type[0] : -1;
	}
	int GetQuestType1()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_type[1] : -1;
	}
	int GetPrizeNPCIndex()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_prizeNPC : -1;
	}
	int GetPrizeNPCZone()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_prizeNpcZoneNum : -1;
	}

	bool IsTutorialQuest()
	{
		return (m_pQuestProto && m_pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL) ? true : false;
	}
	bool IsPetQuest()
	{
		return (	m_pQuestProto->m_index ==  PET_HORSE_QUEST_INDEX
					||	m_pQuestProto->m_index ==  PET_DRAGON_QUEST_INDEX
					||	m_pQuestProto->m_index ==  PET_BLUE_HORSE_QUEST_INDEX
					||	m_pQuestProto->m_index ==  PET_PINK_DRAGON_QUEST_INDEX
					||	m_pQuestProto->m_index ==  PET_UNKOWN_HORSE_QUEST_INDEX
					||	m_pQuestProto->m_index ==  PET_UNKOWN_DRAGON_QUEST_INDEX);
	}
	void SetComplete1(bool bComplete1)
	{
		m_bComplete1 = bComplete1;
	}
	void SetComplete2(bool bComplete2)
	{
		m_bComplete2 = bComplete2;
	}
	bool GetComplete1()
	{
		return m_bComplete1;
	}
	bool GetComplete2()
	{
		return m_bComplete2;
	}
	bool IsCompleted()
	{
		return (m_bComplete1 && m_bComplete2);
	}

	void IncreaseQuestValue(int idx, int val = 1)
	{
		if (idx >= 0 && idx < QUEST_MAX_CONDITION) m_nQuestValue[idx] += val;
	}
	void SetQuestValue(int idx, int val)
	{
		if (idx >= 0 && idx < QUEST_MAX_CONDITION) m_nQuestValue[idx] = val;
	}
	int GetQuestValue(int idx)
	{
		return (idx >= 0 && idx < QUEST_MAX_CONDITION) ? m_nQuestValue[idx] : 0;
	}

	void QuestUpdateData(CPC* pPC, CNPC* pNPC);
	void QuestUpdateData(CPC* pPC, CPC* tPC);

	void RemoeQuestItem(CPC* pc);

	void QuestUpdateDataForParty(CPC* pPC, CNPC* pNPC);
	void QuestUpdateDataForParty(CPC* pPC, CNPC* pNPC, int questIndex);

	int GetQuestType2()
	{
		return (m_pQuestProto) ? m_pQuestProto->m_type[1] : -1;
	}
	int	GetPartyScale()
	{
		return m_pQuestProto->m_partyScale;
	}
	bool CheckComplete(CPC * pc);

	void SetFailValue(int values)
	{
		m_nFailValue = values;
	}
	int GetFailValue()
	{
		return m_nFailValue;
	}
	void SetCompleteTime(long time)
	{
		m_nCompleteTime = time;
	}
	long GetCompleteTime()
	{
		return m_nCompleteTime;
	}

private:
	void SetQuestState(char cQuestState)
	{
		m_cQuestState = cQuestState;
	}
};

class CQuestList
{
	CQuest*			m_head;
	int				m_nCountRun;
	int				m_nCountDone;
	int				m_nCountAbandon;
	int				m_nCountComplete;

public:
	CQuestList();
	~CQuestList();

	int GetCountRun()
	{
		return m_nCountRun;
	}
	int GetCountDone()
	{
		return m_nCountDone;
	}
	int GetCountAbandon()
	{
		return m_nCountAbandon;
	}

	void DecreaseQuestComepleteCount()
	{
		if (m_nCountComplete > 0) m_nCountComplete--;
	}
	void IncreaseQuestComepleteCount()
	{
		m_nCountComplete++;
	}
	int GetQuestComepleteCount()
	{
		return m_nCountComplete;
	}
	void ResetQuestComepleteCount()
	{
		m_nCountComplete = 0;
	}

	CQuest* AddQuest(int nQuestIndex);
	void DelQuest(CPC* pPC, int questIndex);
	bool DelQuest(CPC* pPC, CQuest* pQuest, char cQuestState = QUEST_STATE_RUN );
	void DelQuestALL(CPC* pPC, char cQuestState);
	CQuest* FindQuest(int nQuestIndex);
	bool	FindQuest(int nQuestIndex, char cQuestState);
	CQuest* FindQuestByItem(int nitemIndex);
	CQuest* FindQuestByMob(int nMobIndex);

	bool	IsQuestType0( int qType0 );		//  [2/26/2007 KwonYongDae] 현재 퀘스트 타입이 진행 중인지 확인

	void SetQuestState(CQuest* pQuest, char cQuestState);

	// pCurrentQuest 다음의 cQuestState 상태의 퀘스트를 반환, pCurrentQuest가 NULL이면 처음부터 검색
	CQuest* GetNextQuest(CQuest* pCurrentQuest, char cQuestState = QUEST_STATE_ALL);

	void CheckComplete(CPC* pc);				// 퀘스트 중 Complete 되었는데 Msg 안보냈다면 보낸다 체크!
	void clear();
	bool isRvrQuest(CPC* pc, int questIndex);
};

#endif // !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
