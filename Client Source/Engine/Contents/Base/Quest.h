#ifndef __QUEST__
#define __QUEST__

#include <vector>

class CItems;

#define DEF_MAX_QUEST		16
#define DEF_PAGE_VIEW_COUNT 2

enum eVIEW_PAGE
{
	eVIEW_FIRST = 0,
	eVIEW_SECOND,
	eVIEW_THIRD,
	eVIEW_MAX
};

enum eTREE_TYPE
{
	eTREE_NONE = -1,
	eTREE_PROCEED = 0,
	eTREE_COMPLETE,
	eTREE_RAID,
	eTREE_MAX
};

class ENGINE_API Quest
{
public:
	Quest(void);
	virtual ~Quest(void);

	struct stQuestInfo
	{
		stQuestInfo()
		{
			iQuestIndex		= -1;
			strQuestTitle	= CTString("");
			iQuestType		= -1;
			iQuestScale		= -1;
			dOldTime		= -1;			
			dRemainTime		= -1;
		}

		// 비교 연산자 오버로딩
		inline const bool operator== (const stQuestInfo& rhs)
		{
			return iQuestIndex == rhs.iQuestIndex;
		}		
		// 정렬위한 연산자 오버로딩
		inline const bool operator< (const stQuestInfo& rhs)
		{
			return iQuestIndex < rhs.iQuestIndex;
		}

		CTString	strQuestTitle;
		int			iQuestIndex;
		int			iQuestType;			// 퀘스트 정렬 타입(일반, 시나리오, 외전, 나이트 쉐도우)
		int			iQuestScale;		// 퀘스트 규모(일반, 파티, 원정대)
		DOUBLE		dOldTime;			// 시간제한 퀘스트 한 프레임 전의 시간		
		DOUBLE		dRemainTime;		// 시간제한 퀘스트 남은 시간(초)
	};

	struct stFindQuest
	{
		int iQuestIndex;
		stFindQuest(stQuestInfo& info) : iQuestIndex(info.iQuestIndex) {};
		bool operator() (const stQuestInfo& info)
		{
			if(info.iQuestIndex == iQuestIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	struct stRestoreInfo
	{
		int iQuestIndex;
		int iQuestLevel;
	};

	// 수락 가능, 보상 가능  퀘스트 리스트 (NPC 메뉴에 사용됨)
	BOOL AddQuest( int iQuestIndex, BOOL bComplete );
	BOOL DelQuest( int iQuestIndex, BOOL bComplete );
	void ClearQuest();

	// 퀘스트 북 리스트
	bool IsCompleteQuest(int nQuestIndex);
	void SetQuestRemainTime(int iQuestIndex, int iRemainTime);
	BOOL AddQuestList( int iQuestIndex, BOOL bComplete );
	BOOL DelQuestList( int iQuestIndex, BOOL bComplete );

	void SetExtend(BOOL bExtend, eTREE_TYPE eType) { m_bListExtend[eType] = bExtend; }
	BOOL GetExtend(eTREE_TYPE eType)				 { return m_bListExtend[eType]; }
	void ClearQuestList();
	
	// 레이드 진행 관련
	void AddRaidMessage(const int& iQuestIndex);
	void RemoveRaidMessageAll(void);
	bool isRaidMessage(const int& iQuestIndex);
	void RaidEnd(void);

	DOUBLE	GetQuestNoticeElapsedTime();
	void	SetRaidRemainTime(INDEX iRemainTime);
	DOUBLE	GetRaidReminTime(void);
	BOOL	IsRaidNow(void);

	// 미리보기 관련
	void AddSelectedQuest(const int& iQuestIndex);
	void RemoveSelectedQuest(const int& iQuestIndex);
	void ClearSelectedQuestList();
	bool NextQuestViewPage();
	bool PrevQuestViewPage();
	bool IsSelectedQuest(const int& iQuestIndex);
	int GetSelectedQuestIndex(int Idx);
	int	GetSelectedCount()	{ return m_vectorViewList.size();	}
	int	GetCurrViewPage()	{ return m_nCurrViewPage;			}

	// 퀘스트 복구 관련
	void AddRestoreQuestList(const int& iQuestIndex, int iQuestLevel);
	void ClearAllRestoreList();
	void SortRestoreQuest();

	//////////////////////////////////////////////////////////////////////////
	INDEX GetProceedQuestCount()		{ return m_vectorProceedQuestList.size();	}
	INDEX GetCompleteQuestCount()		{ return m_vectorCompleteQuestList.size();	}
	INDEX GetListProceedCount()			{ return m_vectorListProceed.size();		}
	INDEX GetListCompleteCount()		{ return m_vectorListComplete.size();		}
	INDEX GetListRaidCount()			{ return m_vectorListRaid.size();			}
	INDEX GetRestoreCount()				{ return m_vectorRestoreList.size();		}
	
	const std::vector<int>& GetSelectedQuestList()	{ return m_vectorViewList;		}
	const std::vector<stRestoreInfo>& GetRestoreQuestList()	{ return m_vectorRestoreList;	}
	// 퀘스트 정보 가져오기
	INDEX GetProceedQuestIndex(INDEX index);
	INDEX GetCompleteQuestIndex(INDEX index);
	INDEX GetListProceedIndex(INDEX index);
	INDEX GetListCompleteIndex(INDEX index);
	INDEX GetListRaidIndex(INDEX index);
	CTString GetListProceedTitle(INDEX index)
	{
		return m_vectorListProceed[index].strQuestTitle;
	}
	CTString GetListCompleteTitle(INDEX index)
	{
		return m_vectorListComplete[index].strQuestTitle;
	}
	CTString GetListRaidTitle(INDEX index)
	{
		return m_vectorListRaid[index].strQuestTitle;
	}

	// network
	void SendQuestReq( int nNpcIndex );
	void SendAcceptReq( int nQuestIndex, CItems* pQuestItem = NULL );
	void SendQuestPrize( int iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus );
	void SendQuestCancel( int nQuestIndex );
	
private:
	std::vector<stQuestInfo>	m_vectorProceedQuestList; // 진행가능 퀘스트 리스트
	std::vector<stQuestInfo>	m_vectorCompleteQuestList; // 보상가능 퀘스트 리스트
	
	std::vector<stQuestInfo>	m_vectorListRaid;		//레이드 퀘스트 리스트
	std::vector<stQuestInfo>	m_vectorListProceed;	//진행중 퀘스트 리스트
	std::vector<stQuestInfo>	m_vectorListComplete;	//완료 퀘스트 리스트
	std::vector<stRestoreInfo>	m_vectorRestoreList;	//복구된 퀘스트 리스트
	BOOL						m_bListExtend[eTREE_MAX];

	std::vector<int>			m_vectorViewList;	//선택되어 있는 퀘스트들의 인덱스 모음
	int							m_nCurrViewPage;

	// 퀘스트 공지 시작 시간
	DOUBLE					m_dQuestNoticeStartTime;
	// 퀘스트 공지 두번 안나오게 하기 위해 
	// 이미 발생된 퀘스트 공지를 저장해 둔다	
	std::vector<int>		m_aAlreadyShowQuestNotice;

	// 레이드 시작, 끝 관련
	DOUBLE					m_dRaidRemainTime;	// 레이드 남은 시간
	BOOL					m_bIsRaidNow;		// 레이드 중 인지 판단 플래그
	DOUBLE					m_dRaidRemainTimeRecvTime;	// 레이드 남은 시간을 받은 시간
};

#if		(_MSC_VER >= 1600)
class Compare : std::unary_function<Quest::stRestoreInfo, bool>
{
public:
	bool operator()(const Quest::stRestoreInfo& lhs, const Quest::stRestoreInfo& rhs) const
	{
		bool bResult;

		bResult = lhs.iQuestLevel < rhs.iQuestLevel;
		
		if (lhs.iQuestLevel == rhs.iQuestLevel)
			bResult = lhs.iQuestIndex < rhs.iQuestIndex;
		
		return bResult;
	}
};
#endif	// (_MSC_VER >= 1600)

#endif	//	__QUEST__