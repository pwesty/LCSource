#ifndef _CJEWELDATA_H_
#define _CJEWELDATA_H_

#include <map>

class CJewelData
{
public:
	CJewelData();
	virtual ~CJewelData();
	friend class CJewelDataList;

private:
	int m_jewelGrade;
	GoldType_t m_normalComposeMoney;
	GoldType_t m_chaosComposeMoney;
	int m_normalComposeProb;
	int m_chaosComposeProb;
	int m_composeNormalToChaosProb;
	int m_normal_plus2_prob;
	int m_normal_plus3_prob;
	int m_chaos_plus2_prob;
	int m_chaos_plus3_prob;
	int m_normal_minus1_prob;
	int m_normal_minus2_prob;
	int m_normal_minus3_prob;
	int m_chaos_minus1_prob;
	int m_chaos_minus2_prob;
	int m_chaos_minus3_prob;
	int m_eventComposeProb;					//보석 합성 이벤트 확률 값
	int m_eventChaosCreateProb;				//카오스 보석 생성 이벤트 확률 값
	int m_eventUpgradeProb;					//보석 등급상승 이벤트 확률 값
	int m_eventCombineProb;					//보석 결합 이벤트 확률 값
	int m_eventCollectProb;					//보석 결합 실패시 이벤트 회수 확률 증가 값

public:
	void setEventComposeProb( int composeProb )
	{
		m_eventComposeProb = composeProb;
	}
	void setEventChaosCreateProb( int createProb )
	{
		m_eventChaosCreateProb = createProb;
	}
	void setEventUpgradePorb( int upgradeProb )
	{
		m_eventUpgradeProb = upgradeProb;
	}
	void setEventCombineProb( int combineProb )
	{
		m_eventCombineProb = combineProb;
	}
	void setEventCollectProb( int collectPorb )
	{
		m_eventCollectProb = collectPorb;
	}

	int getJewelGrade()
	{
		return m_jewelGrade;
	}
	int getNormalComposeMoney()
	{
		return m_normalComposeMoney;
	}
	int getChaosComposeMoney()
	{
		return m_chaosComposeMoney;
	}
	int getNormalComposeProb()
	{
		return m_normalComposeProb;
	}
	int getChaosComposeProb()
	{
		return m_chaosComposeProb;
	}
	int getComposeNormalToChaosProb()
	{
		return m_composeNormalToChaosProb;
	}
	int getNormalPlus2Prob()
	{
		return m_normal_plus2_prob;
	}
	int getNormalPlus3Prob()
	{
		return m_normal_plus3_prob;
	}
	int getChaosPlus2Prob()
	{
		return m_chaos_plus2_prob;
	}
	int getChaosPlus3Prob()
	{
		return m_chaos_plus3_prob;
	}
	int getNormalMinus1Prob()
	{
		return m_normal_minus1_prob;
	}
	int getNormalMinus2Prob()
	{
		return m_normal_minus2_prob;
	}
	int getNormalMinus3Prob()
	{
		return m_normal_minus3_prob;
	}
	int getChaosMinus1Prob()
	{
		return m_chaos_minus1_prob;
	}
	int getChaosMinus2Prob()
	{
		return m_chaos_minus2_prob;
	}
	int getChaosMinus3Prob()
	{
		return m_chaos_minus3_prob;
	}
	int getEventComposeProb()
	{
		return m_eventComposeProb;
	}
	int getEventCreateChaosProb()
	{
		return m_eventChaosCreateProb;
	}
	int getEventUpgradeProb()
	{
		return m_eventUpgradeProb;
	}
	int getEventCombineProb()
	{
		return m_eventCombineProb;
	}
	int getEventCollectProb()
	{
		return m_eventCollectProb;
	}
};

class CJewelDataList
{
public:
	typedef std::map<int, CJewelData*> map_t;

	map_t m_jewelDataList;			//보석 데이터 리스트 (key : 보석 등급)
	CJewelData* m_jewelData;		//보석 아이템
	//int m_nCount;
	int **m_gradedJewel;			//등급별 보석 종류
	int **m_gradedChaosJewel;		//등급별 카오스 보석 종류
	int m_nJewelItemKindCount;		//보석 종류 개수
	int m_nJewelItemKindCount_Chaos; //카오스 보석 종류 개수
	int m_JewelPocketItem[JEWEL_POCKET_MAX_LEVEL];
	int m_ChaosJewelPocketItem[JEWEL_POCKET_MAX_LEVEL];

public:
	CJewelDataList();
	virtual ~CJewelDataList();

	bool Load();
	CJewelData* FindGrade(int grade);
	int FindJewelPocketIndex(int grade);
	int FindChaosJewelPocketIndex(int grade);

	int getRandomJewel(int grade, int rand);
	int getRandomChaosJewel(int grade, int rand);
};
#endif //_CJEWELDATA_H_
