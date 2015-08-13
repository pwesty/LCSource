#ifndef _REWARD_SYSTEM_HEADER__
#define _REWARD_SYSTEM_HEADER__

#include "mysql.h"

class CRewardData
{
public:
	CRewardData(int _jobFlag, int _levelMini, int _levelMaxi, int _prob, int _rewardType, int _index, int _value1, int _value2, LONGLONG _value3)
		: m_jobFlag(_jobFlag), m_levelMini(_levelMini), m_levelMaxi(_levelMaxi), m_prob(_prob)
		, m_rewardType(_rewardType), m_index(_index), m_value1(_value1), m_value2(_value2), m_value3(_value3)
	{	}
	~CRewardData() {	}

	const int getJobFlag()
	{
		return m_jobFlag;
	}
	const int getLevelMini()
	{
		return m_levelMini;
	}
	const int getLevelMaxi()
	{
		return m_levelMaxi;
	}
	const int getProb()
	{
		return m_prob;
	}

	const int getRewardType()
	{
		return m_rewardType;
	}

	const int getIndex()
	{
		return m_index;
	}
	const int getValue1()
	{
		return m_value1;
	}
	const int getValue2()
	{
		return m_value2;
	}
	const LONGLONG getValue3()
	{
		return m_value3;
	}

	const int getItemIndex()
	{
		return ( m_rewardType == 0 ? m_index  : -1 );
	}
	const int getItemPlus()
	{
		return ( m_rewardType == 0 ? m_value1 : -1 );
	}
	const int getItemFlag()
	{
		return ( m_rewardType == 0 ? m_value2 : -1 );
	}
	const LONGLONG getItemCount()
	{
		return ( m_rewardType == 0 ? m_value3 : -1 );
	}

	const LONGLONG getNasCount()
	{
		return ( m_rewardType == 1 ? m_value3 : -1 );
	}

	const LONGLONG getExp()
	{
		return ( m_rewardType == 2 ? m_value3 : -1 );
	}

	const LONGLONG getSp()
	{
		return ( m_rewardType == 3 ? m_value3 : -1 );
	}

	const int getStat()
	{
		return ( m_rewardType == 4 ? m_value1 : -1 );
	}

	const int getSkillIndex()
	{
		return ( m_rewardType == 5 ? m_index  : -1 );
	}
	const int getSkillLevel()
	{
		return ( m_rewardType == 5 ? m_value1 : -1 );
	}

	int checkJob( int _job )
	{
		return m_jobFlag & (1<<_job);
	}

protected:

	int	m_jobFlag;
	int m_levelMini;
	int m_levelMaxi;
	int m_prob;
	int m_rewardType;
	int m_index;
	int m_value1;
	int m_value2;
	LONGLONG m_value3;
};

class CReward
{
public:
	CReward(int _rewardIndex, int _probType) : m_rewardIndex(_rewardIndex), m_probType(_probType)
	{	}
	~CReward()
	{
		m_rewardDataList.clear();
	}

	void pushData( CRewardData _rewardData)
	{
		m_rewardDataList.push_back(_rewardData ) ;
	}

	const int getRewardCount()
	{
		return (int)m_rewardDataList.size();
	}
	CRewardData * getRewardData(int _pos);

	const int getProbType()
	{
		return m_probType;
	}
	const int getRewardIndex()
	{
		return m_rewardIndex;
	}

private:

	int m_rewardIndex;
	int m_probType;

	// 보상 리스트
	std::vector<CRewardData> m_rewardDataList;
};

// 보상 데이터 구조체
typedef struct __reward_data_info
{
	int type;
	int index;
	int value1;
	int value2;
	LONGLONG value3;
	__reward_data_info()
	{
		type	= -1;
		index	= 0;
		value1	= 0;
		value2	= 0;
		value3	= 0;
	}
} REWAD_DATA_INFO;

class CRewardManager
{
public:
	CRewardManager()
	{	}
	~CRewardManager()
	{
		initRewardDataInfo();
		m_dataList.clear();
	}

	// DB에서 데이터를 로딩한다.
	bool loadReward(MYSQL * _dataDB);

	// 보상데이트를 먼저 추출하여 보상이 가능한지 판단한다.
	bool setRewardData(CPC * _pCh, int _rewardIndex, int _prob = -1, int _pos = -1 );

	// setRewardData() 에서 만들어진 리스트를 pc에게 지급한다.
	bool doReward( CPC * _pCh, int _rewardIndex);

	const std::vector<REWAD_DATA_INFO> * getRewardDataList()
	{
		return &m_rewardDataList;
	}

	int getRewardDataCount()
	{
		return m_rewardDataList.size();
	}
	int getRewardDataType(int _pos);
	int getRewardDataIndex(int _pos);
	int getRewardDataValue1(int _pos);
	int getRewardDataValue2(int _pos);
	int getRewardDataValue3(int _pos);

protected:

	// 조건에 맞는 넘은 모두 지급 하는 방식
	bool setRewardDataMulti		(CPC * _pCh, CReward * _rward, int _prob);

	// 확률에 따른 한개만 지급하는 방식
	bool setRewardDataOnceRandom(CPC * _pCh, CReward * _rward, int _porb);

	// 리스트중 한개만 선택하여 지급 하는 방식
	bool setRewardDataOnceSelect(CPC * _pCh, CReward * _rward, int _pos);

	// 보상 목록 리스트를 초기화 한다.
	void initRewardDataInfo()
	{
		m_rewardDataList.clear();
		m_invenSlot = 0;
	}

	// 아이템, 스킬 생성이 가능한지 proto 리스트에서 확인
	bool checkProto( CRewardData * _rData );

	// 레벨 및 직업 조건등을 확인
	bool checkRewardCondition(CPC * _pCh, CRewardData * _rData, int _prob, bool _checkRandom=true);

	// 보상 목록에 데이터를 밀어 넣는다.
	bool pushRewardData			( CRewardData * _rData);
	bool pushRewardDataItem		( CRewardData * _rData);

	// pc에기 보상품을 지급한다.
	bool reward2Pc	( CPC * _pCh );
	bool item2PC	( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);
	bool nas2PC		( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);
	bool exp2PC		( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);
	bool sp2PC		( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);
	bool stat2PC	( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);
	bool skill2PC	( CPC * _pCh, REWAD_DATA_INFO* _rewarddata);

private:

	std::map<int, CReward> m_dataList;
	std::vector<REWAD_DATA_INFO> m_rewardDataList;

	int m_invenSlot;
};


#endif // _REWARD_SYSTEM_HEADER__
//
