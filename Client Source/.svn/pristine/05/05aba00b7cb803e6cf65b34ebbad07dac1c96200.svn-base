#ifndef __QUEST_SYSTEM__
#define __QUEST_SYSTEM__

#include <string>
#include <vector>
#include <map>
#include <Engine/Base/Types.h>
#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIDefine.h>

#include <Engine/Help/LoadLod.h>
#include <common/header/def_quest.h>

#define QUEST_FLAG_NOT_LEVEL	0		// 수행 불가능 : 레벨
#define QUEST_FLAG_NOT_JOB		1		// 수행 불가능 : 직업
#define QUEST_FLAG_NOT_ITEM		2		// 수행 불가능 : 아이템
#define QUEST_FLAG_CAN			3		// 수행 가능
#define QUEST_FLAG_ING			4		// 수행 중
#define QUEST_FLAG_COMPLETE		5		// 퀘스트 완료.
#define QUEST_FLAG_NOT_EXP		6		//수행 불가능 : 경험치 부족.
#define QUEST_FLAG_NOT_PREQUEST	7		//수행 불가능 : 선행 퀘스트 미완료.

//서버에서 갖고온 Define
#define		MAX_MAX_NEED_ITEM			5		// 필요아이템 최대 5
#define		QUEST_MAX_CONDITION			3		// 수행조건 최대 3
#define		QUEST_MAX_PRIZE				5		// 보상 최대 5
#define		QUEST_MAX_OPTION_PRIZE		7		// 옵션 보상 최대 7 [090728: selo]
#define		QUEST_MAX_CONDITION_DATA	4		// 수행조건 하나당 부가 데이타 4
#define		MAX_PC_QUEST				10		// 캐릭터 멀티 퀘스트 10

// 퀘스트 분류 [090603: selo]
#define		QCATEGORY_NORMAL			0		// 일반
#define		QCATEGORY_SCENARIO			1		// 시나리오
#define		QCATEGORY_SUPPLEMENT		2		// 외전
//#define		QCATEGORY_NIGHTSHADOW		3		// 나이트 쉐도우
#define		QCATEGORY_END				QCATEGORY_SUPPLEMENT + 1

// 퀘스트 규모 [090603: selo]
#define		QSCALE_SINGLE				0		// 일반
#define		QSCALE_PARTY				1		// 파티
#define		QSCALE_EXPEDITION			2		// 원정대

// 종류별 타입(Type1)
#define		QTYPE_KIND_REPEAT					0  // 반복형 퀘스트
#define		QTYPE_KIND_COLLECTION				1  // 수집형 퀘스트
#define		QTYPE_KIND_DELIVERY					2  // 전달형 퀘스트
#define		QTYPE_KIND_DEFEAT					3  // 격파 퀘스트(싱글던전1)
#define		QTYPE_KIND_SAVE						4  // 구출 퀘스트(싱글던전2)
#define		QTYPE_KIND_MINING_EXPERIENCE		5  // 채굴 체험 퀘스트
#define		QTYPE_KIND_GATHERING_EXPERIENCE		6  // 채집 체험 퀘스트
#define		QTYPE_KIND_CHARGE_EXPERIENCE		7  // 차지 체험 퀘스트
#define		QTYPE_KIND_PROCESS_EXPERIENCE		8  // 가공 체험 퀘스트
#define		QTYPE_KIND_MAKE_EXPERIENCE			9  // 제작 체험 퀘스트
#define		QTYPE_KIND_TUTORIAL					10 // 튜토리얼 퀘스트
#define		QTYPE_KIND_SEARCH					11 // 탐색 퀘스트

// 수행별 타입(Type2)
#define		QTYPE_REPEAT_ONCE			0		// 1회용 퀘스트
#define		QTYPE_REPEAT_UNLIMITED		1		// 무한 퀘스트
#define		QTYPE_REPEAT_DAY			2		// 일일 퀘스트

// 시작 타입
#define		QSTART_NPC					0		// NPC를 통한 시작
#define		QSTART_LEVEL				1		// 레벨 업에 의해 시작
#define		QSTART_ITEM					2		// 아이템을 통한 시작
#define		QSTART_AREA					3		// 특정지역에 도달하여 시작

// 퀘스트 수행 조건
#define		QCONDITION_NPC				0		// NPC
#define		QCONDITION_ITEM				1		// 아이템
#define		QCONDITION_ITEM_NORMAL		2		// 아이템(일반)
#define		QCONDITION_PC				3		// PC (캐릭터 PK)
#define		QCONDITION_AREA				4		// 영역 도달
#define		QCONDITION_ITEMUSE			5		// 아이템 사용

// 퀘스트 보상 종류
#define		QPRIZE_ITEM					0		// 아이템 (돈포함)
#define		QPRIZE_MONEY				1		// 아이템 (돈포함)
#define		QPRIZE_EXP					2		// 경험치
#define		QPRIZE_SP					3		// 스킬포인트
#define		QPRIZE_SKILL				4		// 스킬
#define		QPRIZE_SSKILL				5		// 특수스킬
#define		QPRIZE_RVR_POINT			7		// 결사대 포인트.

#define		QUEST_MAX_NPC				50		// npc 하나당 최대 퀘스트 보유 수 50
#define		QUEST_MAX_PC_COMPLETE		100		// pc가 완료한 1회용 퀘스트 최대 수

// Line count of description
#define	QUEST_TITLE_DESC				4
#define	QUEST_STATUS_DESC				QUEST_MAX_CONDITION * 2
#define	QUEST_PRIZE_DESC				QUEST_MAX_PRIZE + 1
#define QUEST_OPTION_PRIZE_DESC			QUEST_MAX_OPTION_PRIZE + 1	// [090728: selo] 옵션 보상 개수가 변경 되었음 (5개 -> 7개)
#define	QUEST_NEED_DESC					MAX_MAX_NEED_ITEM + 1

// [100119: selo] 보상의 옵션 처리를 위한 구조체
struct stOptionPrizeTemp
{
	INDEX	m_iOptionPrizeType[QUEST_MAX_OPTION_PRIZE];
	INDEX	m_iOptionPrizeIndex[QUEST_MAX_OPTION_PRIZE];
	INDEX	m_iOptionPrizeData[QUEST_MAX_OPTION_PRIZE];
	INDEX	m_iOptionPrizePlus[QUEST_MAX_OPTION_PRIZE];
};

// quest 관련 데이터, DB에서 뽑은 Data로 파일에서 읽어들임. 서버 맘대로임.
class CQuestStaticData : public stQuest, public LodLoader<CQuestStaticData>
{
protected:
	INDEX	m_iCategory;		// 퀘스트 분류 [090603: selo]
	int		transFlag;	//[sora] 미번역 스트링 index 표시

	std::string	m_szName;
	std::string	m_szDesc;
	std::string	m_szDesc2;
	std::string	m_szDesc3;

public:
	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();
	INDEX GetIndex()	{ return index; }

	void SetCategory(INDEX iCategory) { m_iCategory = iCategory; }
	void SetTitle(const char* str)	{ m_szName = str; }
	void SetDesc1(const char* str)	{ m_szDesc = str; }
	void SetDesc2(const char* str)	{ m_szDesc2 = str; }
	void SetDesc3(const char* str)	{ m_szDesc3 = str; }

	INDEX GetCategory() { return m_iCategory; }
	const char* GetTitle()	{ return m_szName.c_str(); }
	const char* GetDesc1()	{ return m_szDesc.c_str(); }
	const char* GetDesc2()	{ return m_szDesc2.c_str(); }
	const char* GetDesc3()	{ return m_szDesc3.c_str(); }

	static bool loadEx(const char* fileName);
};

// quest 관련 데이터, 서버가 보내주는 Data와 파일에서 읽은 Data를 짬뽕해서 만드는 데이타
// 보기 좋으라고 만드는 Data로 현재 플레이 중인 캐릭터의 Data임. String 위주임.
class ENGINE_API CQuestDynamicData
{
private:
	CQuestStaticData* m_pStaticData;
public:
	CQuestDynamicData(CQuestStaticData* data);
	~CQuestDynamicData();

	// from quest static data
	INDEX GetQuestIndex()	{ return m_iQuestIndex; }
	const char *GetName()	{ return m_pStaticData->GetTitle(); }
	INDEX GetQuestCategory() { return m_pStaticData->GetCategory();	}   const		// 퀘스트 분류 [090603: selo]
	INDEX GetQuestPartyScale() { return m_pStaticData->partyscale; }	const		// 퀘스트 규모 [090603: selo]
	INDEX GetQuestType1()	{ return m_pStaticData->type1;		}	const		// Type1
	INDEX GetQuestType2()	{ return m_pStaticData->type2;		}	const		// Type2

	INDEX GetStartType()	{ return m_pStaticData->startType;	}	const
	INDEX GetStartData()	{ return m_pStaticData->startData;	}	const
	//INDEX GetGiveNPCIndex()	{ return m_rStaticData.m_iGiveNPCIndex; }	const
	INDEX GetPrizeNPCIndex(){ return m_pStaticData->prizeNPC; }	const
	INDEX GetPreQuestIndex(){ return m_pStaticData->preQuestNo; }	const
	INDEX GetStartNPCZoneIndex(){ return m_pStaticData->startNpcZoneNo; }	const
	INDEX GetPrizeNPCZoneIndex(){ return m_pStaticData->prizeNpcZoneNo; }	const

	INDEX GetNeedMinLevel()	{ return m_pStaticData->needMinLevel;	}	const
	INDEX GetNeedMaxLevel()	{ return m_pStaticData->needMaxLevel;	}	const
	INDEX GetNeedJob()		{ return m_pStaticData->needJob;	}	const
	INDEX GetNeedItemIndex(INDEX i)		{ ASSERT(i < MAX_MAX_NEED_ITEM); return m_pStaticData->needItemIndex[i]; }
	INDEX GetNeedItemCount(INDEX i)		{ ASSERT(i < MAX_MAX_NEED_ITEM); return m_pStaticData->needItemCount[i]; }

	INDEX GetConditionType(INDEX i)		{ ASSERT(i < QUEST_MAX_CONDITION); return m_pStaticData->conditionType[i]; }
	INDEX GetConditionIndex(INDEX i)	{ ASSERT(i < QUEST_MAX_CONDITION); return m_pStaticData->conditionIndex[i]; }
	INDEX GetConditionNum(INDEX i)		{ ASSERT(i < QUEST_MAX_CONDITION); return m_pStaticData->conditionNum[i]; }
	INDEX GetConditionData(INDEX i, INDEX j)		
	{ ASSERT(i < QUEST_MAX_CONDITION); ASSERT(j < QUEST_MAX_CONDITION_DATA); return m_pStaticData->conditionData[i][j]; }

	INDEX		GetSyndicateType()	{ return m_pStaticData->rvr_type;	}
	INDEX		GetSyndicateGrade()	{ return m_pStaticData->rvr_grade;	}
	
	//INDEX GetPrizeType(INDEX i)			{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iPrizeType[i]; }
	//INDEX GetPrizeIndex(INDEX i)		{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iPrizeIndex[i]; }
	//INDEX GetPrizeData(INDEX i)			{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iPrizeData[i]; }
	//INDEX IsOptionPrizeExist()			{ return m_rStaticData.m_bOptionPrize;	}
	//INDEX GetOptionPrizeType(INDEX i)	{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iOptionPrizeType[i]; }
	//INDEX GetOptionPrizeIndex(INDEX i)	{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iOptionPrizeIndex[i]; }
	//INDEX GetOptionPrizeData(INDEX i)	{ ASSERT(i < QUEST_MAX_PRIZE); return m_rStaticData.m_iOptionPrizeData[i]; }
	INDEX IsPrizeExist()				{ return m_bPrize;	}
	INDEX GetPrizeType(INDEX i)			{ ASSERT(i < QUEST_MAX_PRIZE); return m_iPrizeType[i]; }
	INDEX GetPrizeIndex(INDEX i)		{ ASSERT(i < QUEST_MAX_PRIZE); return m_iPrizeIndex[i]; }
	__int64 GetPrizeData(INDEX i)			{ ASSERT(i < QUEST_MAX_PRIZE); return m_iPrizeData[i]; }

	INDEX IsOptionPrizeExist()			{ return m_bOptionPrize;	}
	INDEX GetOptionPrizeType(INDEX i)	{ ASSERT(i < QUEST_MAX_OPTION_PRIZE); return m_iOptionPrizeType[i]; }
	INDEX GetOptionPrizeIndex(INDEX i)	{ ASSERT(i < QUEST_MAX_OPTION_PRIZE); return m_iOptionPrizeIndex[i]; }
	INDEX GetOptionPrizeData(INDEX i)	{ ASSERT(i < QUEST_MAX_OPTION_PRIZE); return m_iOptionPrizeData[i]; }
	INDEX GetOptionPrizePlus(INDEX i)	{ ASSERT(i < QUEST_MAX_OPTION_PRIZE); return m_iOptionPrizePlus[i]; }

	const char *GetDesc()	{ return m_pStaticData->GetDesc1(); }
	const char *GetDesc2()	{ return m_pStaticData->GetDesc2(); }
	const char *GetDesc3()	{ return m_pStaticData->GetDesc3(); }		

	// myself
	void SetCurrentConditionValueStatus(INDEX current[QUEST_MAX_CONDITION]);
	INDEX GetCurrentConditionValueStatus(INDEX i)	{ ASSERT(i < QUEST_MAX_CONDITION); return m_iCurrentConditionValueStatus[i]; }

	void MakeQuestComplete()			{ m_bQuestComplete = TRUE; }
	void MakeQuestUncomplete()			{ m_bQuestComplete = FALSE; }
	BOOL IsQuestComplete()				{ return m_bQuestComplete; }

	//INDEX GetConditionValue(INDEX i)	{ ASSERT(i < QUEST_MAX_CONDITION); return m_iConditionValue[i]; }
	//INDEX GetPrizeValue(INDEX i)		{ ASSERT(i < QUEST_MAX_PRIZE); return m_iPrizeValue[i]; }

	CTString	&GetIntroDesc() { return m_strIntroDesc; }
	COLOR		GetColorIntroDesc() { return m_colIntroDesc; }

	INDEX		GetCountTitleDesc() { return m_ctTitleDesc; }
	CTString	&GetTitleDesc( INDEX iIndex ) { return m_strTitleDesc[iIndex]; }
	COLOR		GetColorTitleDesc( INDEX iIndex ) { return m_colTitleDesc[iIndex]; }

	INDEX		GetCountStatusDesc() { return m_ctStatusDesc; }
	CTString	&GetStatusDesc( INDEX iIndex ) { return m_strStatusDesc[iIndex]; }
	COLOR		GetColorStatusDesc( INDEX iIndex ) { return m_colStatusDesc[iIndex]; }

	INDEX		GetCountPrizeDesc() { return m_ctPrizeDesc; }
	CTString	&GetPrizeDesc( INDEX iIndex ) { return m_strPrizeDesc[iIndex]; }
	COLOR		GetColorPrizeDesc( INDEX iIndex ) { return m_colPrizeDesc[iIndex]; }

	INDEX		GetCountOptionPrizeDesc() { return m_ctOptionPrizeDesc; }
	CTString	&GetOptionPrizeDesc( INDEX iIndex ) { return m_strOptionPrizeDesc[iIndex]; }
	COLOR		GetColorOptionPrizeDesc( INDEX iIndex ) { return m_colOptionPrizeDesc[iIndex]; }

	INDEX		GetCountNeedDesc() { return m_ctNeedDesc; }
	CTString	&GetNeedDesc( INDEX iIndex ) { return m_strNeedDesc[iIndex]; }
	COLOR		GetColorNeedDesc( INDEX iIndex ) { return m_colNeedDesc[iIndex]; }

	friend class CQuestSystem;
	
protected:
	CQuestDynamicData(const CQuestDynamicData &other);
	CQuestDynamicData &operator = (CQuestDynamicData &other);

	void	MakeQuestFirstDesc();
	void	MakeQuestTitleDesc();
	void	MakeQuestStatusDesc();
	void	MakeQuestPrizeDesc();
	void	MakeQuestOptionPrizeDesc();
	void	MakeQuestNeedDesc();

	INDEX	FindItemForPrize(INDEX type, INDEX index);

	INDEX		m_iQuestIndex;
	BOOL		m_bQuestComplete;

	BOOL		m_bPrize;
	INDEX		m_iPrizeType[QUEST_MAX_PRIZE];
	INDEX		m_iPrizeIndex[QUEST_MAX_PRIZE];
	__int64		m_iPrizeData[QUEST_MAX_PRIZE];
	
	INDEX		m_bOptionPrize;
	INDEX		m_iOptionPrizeType[QUEST_MAX_OPTION_PRIZE];
	INDEX		m_iOptionPrizeIndex[QUEST_MAX_OPTION_PRIZE];
	INDEX		m_iOptionPrizeData[QUEST_MAX_OPTION_PRIZE];
	INDEX		m_iOptionPrizePlus[QUEST_MAX_OPTION_PRIZE];

	// [090728: selo] 옵션 보상 카운트
	INDEX		m_iOptionPrizeCount;

	INDEX		m_iCurrentConditionValueStatus[QUEST_MAX_CONDITION];
	CTString	m_strIntroDesc;
	COLOR		m_colIntroDesc;
	INDEX		m_ctTitleDesc;
	CTString	m_strTitleDesc[QUEST_TITLE_DESC];
	COLOR		m_colTitleDesc[QUEST_TITLE_DESC];
	INDEX		m_ctStatusDesc;
	CTString	m_strStatusDesc[QUEST_STATUS_DESC];
	COLOR		m_colStatusDesc[QUEST_STATUS_DESC];
	INDEX		m_ctPrizeDesc;
	CTString	m_strPrizeDesc[QUEST_PRIZE_DESC];
	COLOR		m_colPrizeDesc[QUEST_PRIZE_DESC];
	INDEX		m_ctOptionPrizeDesc;
	CTString	m_strOptionPrizeDesc[QUEST_OPTION_PRIZE_DESC];
	COLOR		m_colOptionPrizeDesc[QUEST_OPTION_PRIZE_DESC];
	INDEX		m_ctNeedDesc;
	CTString	m_strNeedDesc[QUEST_NEED_DESC];
	COLOR		m_colNeedDesc[QUEST_PRIZE_DESC];
};

// Quest관련 전반적인 관리를 담당한다. 싱글턴 패턴.
class ENGINE_API CQuestSystem
{
public:
	enum eNpcQuestType
	{
		NQT_NONE,
		NQT_HAVE_QUEST,
		NQT_CAN_PRIZE,
		NQT_COUNT,
	};
	enum eQuestAllowType
	{
		QAT_ALLOW,
		QAT_ALREADY_DONE,
		QAT_GIVEUP,
		QAT_DOING,//현재 수행중.
	};
public:
	static CQuestSystem &Instance()	{ return m_instance; }
	bool Load();

	CQuestDynamicData *GetDynamicDataByVectorIndex(INDEX idx)
	{
		if(idx >= m_vectorCurrentUserQuest.size()) return NULL;
		else return m_vectorCurrentUserQuest[idx];
	}
	CQuestDynamicData *GetDynamicDataByQuestIndex(INDEX idxQuest);
	INDEX GetVectorIndexByGiveNPCIndex( INDEX idxNPC );
	INDEX GetVectorIndexByPrizeNPCIndex( INDEX idxNPC );

	CQuestDynamicData *Create(INDEX idxQuest);
	INDEX AddCurrentRequest();
	void DeleteCurrentRequest();
	BOOL Remove(INDEX idxQuest);

	CQuestDynamicData *GetCurrentRequest() const { return m_pCurrentRequest; }

	void	ClearAllDynamicData();
	void	ClearAllAllowList();

	~CQuestSystem();
	
	CQuestStaticData* GetStaticData(INDEX idxQuest)
	{
		return CQuestStaticData::getData(idxQuest);
	}

	void SetQuestAllow(INDEX iQuestIndex, eQuestAllowType qat)
	{
		if(iQuestIndex==-1) return;
		if((qat == QAT_ALREADY_DONE || qat == QAT_GIVEUP)
		&& CQuestStaticData::getData(iQuestIndex)->type2 == QTYPE_REPEAT_UNLIMITED)
		{
			m_vectorQuestAllowList[iQuestIndex] = QAT_ALLOW;
			return;
		}
		m_vectorQuestAllowList[iQuestIndex] = qat;
	}
	eQuestAllowType GetQuestAllow(INDEX iQuestIndex)
	{
		ASSERT(iQuestIndex != -1);
		return m_vectorQuestAllowList[iQuestIndex];
	}
	const char *GetQuestName(int iQuestIndex)
	{
		return CQuestStaticData::getData(iQuestIndex)->GetTitle();
	}
	const int GetQuestMinLevel(int iQuestIndex)
	{
		return CQuestStaticData::getData(iQuestIndex)->needMinLevel;
	}
	const int GetQuestMaxLevel(int iQuestIndex)
	{
		return CQuestStaticData::getData(iQuestIndex)->needMaxLevel;
	}

	INDEX SearchContactQuestIndex(INDEX iQuestIndex); // 퀘스트 인덱스와 연계 되는 퀘스트 검색 리턴 값은 연계되는 npc Index
	BOOL CanIDoQuest(INDEX iQuestIndex);
	CTString MakeInfoForCondition(int iQuestIndex, int iConditionType, int iConditionIndex, int iCurrentCnt, int iConditionCnt);
	eNpcQuestType TestNPCForQuest(INDEX iNPCIndex);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate( std::vector<INDEX> transList );
	void ClearNoTranslate( std::vector<INDEX> transList );
	
protected:
	CQuestSystem();

	static CQuestSystem m_instance;
	CQuestDynamicData *m_pCurrentRequest;
	std::vector<CQuestDynamicData*> m_vectorCurrentUserQuest;
	std::vector<eQuestAllowType>	m_vectorQuestAllowList;
	typedef std::multimap<INDEX, INDEX> mmap;
	mmap m_mmapNPCToQuest;
	mmap m_mmapPreQuestToCurQuest;
};

#endif //__QUEST_SYSTEM__