#ifndef __RANKINGSYSTEM_H__
#define __RANKINGSYSTEM_H__

#include "mysql.h"

class CRankingData
{
public:
	// 캐릭터 정보
	int m_index;		// char index
	CLCString m_name;	// char name
	int m_level;		// char level
	int m_job;			// char job

	// 랭킹 정보
	int m_rank;			// 현재 랭킹
	int m_rank_pre;		// 이전 랭킹

	// 달성값
	int m_value;				// 현재 달성값
	int m_value_pre;			// 이전 달성값
	CLCString m_achieve_date;	// 달성 날짜
};

class CRankingSystemEX
{
	class CRankerData
	{
		// 랭커 목록 보낼 때 사용되는 데이터
	public:
		CRankerData()
			: m_char_index(0), m_rank(0), m_rank_pre(0), m_value(0), m_value_pre(0), m_level(0), m_job(0)
		{
		}
		~CRankerData() {};

		CRankerData& operator=(const CRankerData& other)
		{
			this->m_char_index = other.m_char_index;
			this->m_rank = other.m_rank;
			this->m_rank_pre = other.m_rank_pre;
			this->m_value = other.m_value;
			this->m_value_pre = other.m_value_pre;
			this->m_level = other.m_level;
			this->m_job = other.m_job;
			this->m_name = other.m_name;
			this->m_date = other.m_date;

			return *this;
		}
		CLCString m_name, m_date;
		int m_char_index;
		int m_rank;
		int m_rank_pre;
		int m_value;
		int m_value_pre;
		int m_level;
		int m_job;
	};
private:
	std::string		m_refreshDate;			/* 랭킹 갱신 시간 */
	bool m_lock;
	bool m_reward;	// 보상줄게 있는지? ( true : 보상을 줘야할게 있음, false : 보상을 줘야할게 없음 )

public:
	CRankingSystemEX();
	~CRankingSystemEX();

	MYSQL m_dbrank;
	bool TestConnectDB();
	bool ConnectDB();

	// 갱신 관련
	void StartRefresh(void);					// 랭킹 갱신 시작
	bool RankingEX_Refresh(void);				// 갱신
	// 보상 관련
	void RankingEX_GiveReward(int endTime);		// 보상 지급
	// 시간 관련
	void CheckRankingRefreshTime(void);			// 랭킹 갱신 시간 확인
	// 락 관련
	bool IsLock(void)
	{
		return m_lock;	   // 랭킹 시스템 락
	}
	void SetLock(bool b)
	{
		m_lock = b;		   // 랭킹 시스템 락
	}
	// 보상 여부 관련
	bool IsExistReward(void)
	{
		return m_reward;	   // 보상 여부 ( true : 보상을 줘야할게 있음, false : 보상을 줘야할게 없음 )
	}
	void SetExistReward(bool b)
	{
		m_reward = b;		   // 보상 여부 ( true : 보상을 줘야할게 있음, false : 보상을 줘야할게 없음 )
	}
	// 리스트 관련
	void RankingEX_TOP50List(CNetMsg::SP& msg, CDescriptor* dest, int type, int subtype);	// Top50 랭커 리스트 얻기
	void RankingEX_SearchList(CNetMsg::SP& msg, CDescriptor* dest, int type, int subtype);	// Search 랭커 리스트 얻기 (랭커를 검색하면 해당 랭커 아래,위 5명씩 총 11명의 리스트를 보내준다)
private:
	// Refresh 관련
	bool RankingEX_BackUp(void);				// 백업
	bool RankingEX_ClearRankingTable(void);		// 랭킹 테이블 초기화
	bool RankingEX_ClearCharTable(void);		// 캐릭터 테이블 랭킹 관련 필드 초기화
	bool RankingEX_RefreshData(void);			// t_characters -> t_ranking_data
	bool RankingEX_RefreshRankByLevel(void);	// 랭킹 산정
	bool RankingEX_RefreshDate(void);			// 갱신 날짜 기록
	std::string GetRefreshTime(void);				// 갱신 시간 확인

	// 보상 관련
	void RankingEX_GiveRewardByType(int type, int subtype, int endTime);	// 랭커 타입에 따라 보상 지급하기
	int RankingEX_GetRewardTitle(int type, int subtype);					// 보상 아이템(호칭) 인덱스 얻기

	// 기타
	std::string GetPreviousRefreshDate(void);						// 이전 랭킹 갱신 날짜 얻기
	int GetRankerCount(int type, int subtype);						// 랭커의 총 수 얻기
	int GetRankingByName(int type, int subtype, CLCString name);	// 캐릭터 이름으로 랭킹 얻기
};

#endif // __RANKINGSYSTEM_H__
//
