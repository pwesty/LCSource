#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Reward.h"
#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"

CRewardData * CReward::getRewardData(int _pos)
{
	if( m_rewardDataList.size()==0 ||  _pos < 0 || (int)m_rewardDataList.size() <= _pos )
		return NULL;

	return &m_rewardDataList[_pos];
}

bool CRewardManager::loadReward(MYSQL* _dataDB)
{
	CDBCmd cmdHead;
	cmdHead.Init(_dataDB);
	std::string select_reward_head_query = "SELECT * FROM t_reward_head ORDER BY a_reward_idx";
	cmdHead.SetQuery(select_reward_head_query);

	if( !cmdHead.Open() || !cmdHead.MoveFirst() )
		return false;

	if( cmdHead.m_nrecords < 1 )
		return false;

	CDBCmd cmdData;
	cmdData.Init(_dataDB);

	int rewardIndex;
	int probType;
	do
	{
		cmdHead.GetRec("a_reward_idx", rewardIndex);
		cmdHead.GetRec("a_rand_type", probType);

		std::string select_reward_data_query = boost::str(boost::format("SELECT * FROM t_reward_data WHERE a_reward_idx=%d ORDER BY a_primarykey") % rewardIndex);

		cmdData.SetQuery( select_reward_data_query );

		if( !cmdData.Open()  )
			return false;

		if( !cmdData.MoveFirst() )
			continue;

		if( cmdData.m_nrecords < 1 )
			continue;

		int type;
		int index;
		int value1, value2;
		LONGLONG value3;
		int jobFlag;
		int mini, maxi;
		int prob;

		CReward reward(rewardIndex, probType);
		do
		{
			if( !cmdData.GetRec("a_type", type)
					|| !cmdData.GetRec("a_idx", index)
					|| !cmdData.GetRec("a_value_1", value1)
					|| !cmdData.GetRec("a_value_2", value2)
					|| !cmdData.GetRec("a_value_3", value3)
					|| !cmdData.GetRec("a_job_flag", jobFlag)
					|| !cmdData.GetRec("a_level_mini", mini)
					|| !cmdData.GetRec("a_level_maxi", maxi)
					|| !cmdData.GetRec("a_prob", prob)
			  )
				return false;
			CRewardData rewardData(jobFlag, mini, maxi, prob, type, index, value1, value2, value3);
			reward.pushData( rewardData );
		}
		while( cmdData.MoveNext() );

		m_dataList.insert( std::map<int,CReward>::value_type( rewardIndex, reward ) );
	}
	while( cmdHead.MoveNext() );

	return true;
}

bool CRewardManager::doReward(CPC * _pCh,  int _rewardIndex )
{
	// 보상 대상자가 없다.
	if( !_pCh )
		return false;

	// 보상 인데스가 유효하지 않음
	if( _rewardIndex < 1 )
		return false;

	// 보상 리스트가 없다.
	if( m_dataList.size() == 0 )
		return false;

	// 보상 데이터 찾기
	std::map<int, CReward>::iterator itr = m_dataList.find( _rewardIndex);

	// 보상 인덱스의 보상 데이터가 없다.
	if( itr == m_dataList.end() )
		return false;

	// 보상 데이타를 기지고 놀자.
	CReward rData = itr->second;

	// 아이템 지급
	GAMELOG << init("REWARD-START", _pCh ) 	<< "reward index : " <<itr->second.getRewardIndex() << end;
	if( !reward2Pc(_pCh) )
	{
		GAMELOG << init("REWARD-FAIL ", _pCh ) 	<< "reward index : " <<itr->second.getRewardIndex() << end;
		return false;
	}
	GAMELOG << init("REWARD-END  ", _pCh ) 	<< "reward index : " <<itr->second.getRewardIndex() << end;
	return true;
}

bool CRewardManager::checkRewardCondition(CPC * _pCh, CRewardData * _rData, int _prob, bool _checkRandom)
{
	// 지급 가능한 잡 확인
	if( !_rData->checkJob(_pCh->m_job ) )
		return false;

	// 레벨 확인
	if(_rData->getLevelMini() > _pCh->m_level || _rData->getLevelMaxi() < _pCh->m_level )
		return false;

	// 확률
	if( _checkRandom )
	{
		int prob = GetRandom(0,10000);

		if( _prob > 0 )
			prob = _prob;

		if( _rData->getProb() < prob)
			return false;
	}

	return true;
}

bool CRewardManager::checkProto( CRewardData * _rData )
{
	switch( _rData->getRewardType() )
	{
	case 0:
		{
			CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex( _rData->getItemIndex() );
			if( !pItemProto )
				return false;
			pItemProto = NULL;
		}
		break;
	case 5:
		{
			const CSkillProto* proto = gserver->m_skillProtoList.Find(_rData->getSkillIndex());
			if( !proto )
				return false;

			if( proto->m_maxLevel < _rData->getSkillLevel() )
			{
				proto = NULL;
				return false;
			}
			proto = NULL;
		}
		break;
	}

	return true;
}

bool CRewardManager::setRewardData(CPC * _pCh, int _rewardIndex,int _prob, int _pos)
{
	// 보상 아이템을 담을 메모리 초기화
	initRewardDataInfo();

	// 보상 데이터 찾기
	std::map<int, CReward>::iterator itr = m_dataList.find( _rewardIndex);

	// 보상 인덱스의 보상 데이터가 없다.
	if( itr == m_dataList.end() )
		return false;

	// 보상 데이타를 기지고 놀자.
	CReward rData = itr->second;

	switch( rData.getProbType() )
	{
	case 0: // multi
		if( !setRewardDataMulti( _pCh, &rData, _prob) )
			return false;
		break;
	case 1: // once 아이템중에 하나만 골라 지급 한다.
		if( !setRewardDataOnceRandom( _pCh, &rData, _prob) )
			return false;
		break;
	case 2: // once(selected) 선택된 하나만 지급.
		if( !setRewardDataOnceSelect( _pCh, &rData, _pos) )
			return false;
		break;
	default:
		return false;
	}

	if( rData.getProbType() != 0 && m_rewardDataList.size() == 0 )
	{
		initRewardDataInfo();
		return false;
	}

	// 인벤 빈 슬롯과 무게 확인
	if( _pCh->m_inventory.getEmptyCount() < m_invenSlot )
	{
		initRewardDataInfo();
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q(rmsg, _pCh->m_desc);
		return false;
	}

	return true;
}

bool CRewardManager::setRewardDataOnceSelect(CPC * _pCh, CReward * _rward, int _pos)
{
	int count = _rward->getRewardCount();
	if( count == 0 )
		return true;

	int pos = _pos;

	if( count <= pos || pos < 0 )
		pos = GetRandom(0, 1000) % count;

	CRewardData * rData = _rward->getRewardData( pos);

	if( !rData )
		return false;

	pushRewardData( rData );
	rData = NULL;
	return true;
}

bool CRewardManager::setRewardDataOnceRandom(CPC * _pCh, CReward * _rData, int _prob)
{
	int count = _rData->getRewardCount();
	if( count == 0 )
		return true;

	int totalProb=0;
	int prob = GetRandom(1, 10000);
	if( _prob > 0 )
		prob = _prob;

	int i=0;
	for(i=0; i<count; i++)
	{
		CRewardData * rData = _rData->getRewardData(i);

		if( !rData )
			return false;

		if( !checkProto( rData ) )
			continue;

		totalProb += rData->getProb();
		if( prob > totalProb )
			continue;

		if( checkRewardCondition(_pCh, rData, _prob, false ) )
		{
			if( !pushRewardData( rData ) )
				return false;
			return true;
		}
		continue;
	}
	return false;
}

bool CRewardManager::setRewardDataMulti(CPC * _pCh, CReward * _rData, int _prob)
{
	int count = _rData->getRewardCount();
	if( count == 0 )
		return true;

	int i=0;
	for(i=0; i<count; i++)
	{
		CRewardData * rData = _rData->getRewardData(i);

		if( !rData )
			return false;

		if( !checkProto( rData ) )
			continue;

		if( !checkRewardCondition(_pCh, rData ,_prob  ) )
			continue;

		if( !pushRewardData( rData ) )
			return false;
	}
	return true;
}

bool CRewardManager::pushRewardData( CRewardData * _rData)
{
	switch( _rData->getRewardType() )
	{
	case 0:  // Item
		return pushRewardDataItem( _rData );
	case 1: // Nas
	case 2: // Exp
	case 3:	// SP
	case 4:	// Stat
	case 5:	// Skill
		{
			REWAD_DATA_INFO rdInfo;
			rdInfo.type		= _rData->getRewardType();
			rdInfo.index	= _rData->getIndex();
			rdInfo.value1	= _rData->getValue1();
			rdInfo.value2	= _rData->getValue2();
			rdInfo.value3	= _rData->getValue3();
			m_rewardDataList.push_back(rdInfo);
			return true;
		}
	}
	return false;
}

bool CRewardManager::pushRewardDataItem( CRewardData * _rData)
{
	CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(_rData->getItemIndex() );

	if( !pItemProto )
		return false;

	if(  pItemProto->getItemFlag() & ITEM_FLAG_COUNT )
		m_invenSlot++; // 카운트가 되는 아이템은 슬롯 하나로 계산
	else
		m_invenSlot += _rData->getItemCount();

	REWAD_DATA_INFO rdInfo;
	rdInfo.type		= _rData->getRewardType();
	rdInfo.index	= _rData->getItemIndex();
	rdInfo.value1	= _rData->getItemPlus();
	rdInfo.value2	= _rData->getItemFlag();
	rdInfo.value3	= _rData->getItemCount();

	if( pItemProto->getItemFlag() & ITEM_FLAG_COUNT )
	{
		m_rewardDataList.push_back(rdInfo);
	}
	else
	{
		// 카운트가 않되는 아이템은 하나씩 넣어놓고 지급한다.
		rdInfo.value3 = 1;

		int j=0;
		int itemCount = _rData->getItemCount();
		for(j=0;  j<itemCount; j++)
		{
			m_rewardDataList.push_back(rdInfo);
		}
	}
	return true;
}

bool CRewardManager::reward2Pc( CPC * _pCh )
{
	if( m_rewardDataList.size() == 0 )
		return true;

	int i=0;
	int rewardCount = (int)m_rewardDataList.size();
	for(i=0; i<rewardCount; i++)
	{
		switch( m_rewardDataList[i].type )
		{
		case 0:
			if( !item2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		case 1:
			if( !nas2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		case 2:
			if( !exp2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		case 3:
			if( !sp2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		case 4:
			if( !stat2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		case 5:
			if( !skill2PC( _pCh, &m_rewardDataList[i] ) )
				return false;
			break;
		}
	}

	return true;
}

bool CRewardManager::item2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
	CItem * pItem = gserver->m_itemProtoList.CreateItem(
						_rewarddata->index, -1, _rewarddata->value1, _rewarddata->value2, _rewarddata->value3);

	if( !pItem )
	{
		GAMELOG << init("REWARD-ITEM FAIL", _pCh) << "INDEX : " << _rewarddata->index << end;
		return false;
	}
	GAMELOG << init("REWARD-ITEM ", _pCh) 	<< itemlog(pItem) << end;

	if (_pCh->m_inventory.addItem(pItem) == false)
	{
		GAMELOG << init("REWARD-ITEM FAIL", _pCh) << itemlog(pItem) << end;

		delete pItem;
		return false;
	}

	return true;
}

bool CRewardManager::nas2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
//	bool Money;
	_pCh->m_inventory.increaseMoney(_rewarddata->value3);
	GAMELOG << init("REWARD-NAS  ", _pCh ) 	<< _rewarddata->value3 << end;
	return true;
}

bool CRewardManager::exp2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
	_pCh->AddExpSP_Direct(_rewarddata->value3, 0 );

	CNetMsg::SP rmsg(new CNetMsg);
	ExpSPMsg(rmsg, _rewarddata->value3, 0);
	SEND_Q(rmsg, _pCh->m_desc);
	GAMELOG << init("REWARD-EXP  ", _pCh ) 	<< _rewarddata->value3 << end;
	return true;
}

bool CRewardManager::sp2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
	_pCh->AddExpSP_Direct( 0,_rewarddata->value3*10000  );

	CNetMsg::SP rmsg(new CNetMsg);
	ExpSPMsg(rmsg, 0, _rewarddata->value3  );
	SEND_Q(rmsg, _pCh->m_desc);
	GAMELOG << init("REWARD-SP   ", _pCh ) 	<< _rewarddata->value3  << end;
	return true;
}

bool CRewardManager::stat2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
	_pCh->m_statpt_remain += _rewarddata->value1;

	CNetMsg::SP rmsg(new CNetMsg);
	StatPointRemainMsg(rmsg, _pCh);
	SEND_Q(rmsg, _pCh->m_desc);
	GAMELOG << init("REWARD-STAT ", _pCh ) 	<< _rewarddata->value1 << end;
	return true;
}

bool CRewardManager::skill2PC( CPC * _pCh, REWAD_DATA_INFO* _rewarddata)
{
	CSkill* pSkill = gserver->m_skillProtoList.Create(_rewarddata->index, _rewarddata->value1 );
	if( !pSkill )
	{
		GAMELOG << init("REWARD-SKILL FAIL", _pCh ) 	<< _rewarddata->index << delim << _rewarddata->value1 << end;
		return false;
	}

	bool bApply;
	ApplySkill(_pCh, _pCh, pSkill, -1, bApply);
	delete pSkill;
	pSkill = NULL;
	GAMELOG << init("REWARD-SKILL", _pCh ) 	<< _rewarddata->index << delim << _rewarddata->value1 << end;
	return true;
}

int CRewardManager::getRewardDataType(int _pos)
{
	//if( (int)m_rewardDataList.size() == 0 || _pos < (int)m_rewardDataList.size() || (int)m_rewardDataList.size() <= _pos )
	if( (int)m_rewardDataList.size() == 0 || _pos >= (int)m_rewardDataList.size() || _pos < 0 )
		return -1;

	return m_rewardDataList[_pos].type;
}

int CRewardManager::getRewardDataIndex(int _pos)
{
	//if( (int)m_rewardDataList.size() == 0 || _pos < (int)m_rewardDataList.size() || (int)m_rewardDataList.size() <= _pos )
	if( (int)m_rewardDataList.size() == 0 || _pos >= (int)m_rewardDataList.size() || _pos < 0 )
		return -1;

	return m_rewardDataList[_pos].index;
}
int CRewardManager::getRewardDataValue1(int _pos)
{
	//if( (int)m_rewardDataList.size() == 0 || _pos < (int)m_rewardDataList.size() || (int)m_rewardDataList.size() <= _pos )
	if( (int)m_rewardDataList.size() == 0 || _pos >= (int)m_rewardDataList.size() || _pos < 0 )
		return -1;

	return m_rewardDataList[_pos].value1;
}
int CRewardManager::getRewardDataValue2(int _pos)
{
	//if( (int)m_rewardDataList.size() == 0 || _pos < (int)m_rewardDataList.size() || (int)m_rewardDataList.size() <= _pos )
	if( (int)m_rewardDataList.size() == 0 || _pos >= (int)m_rewardDataList.size() || _pos < 0 )
		return -1;

	return m_rewardDataList[_pos].value2;
}
int CRewardManager::getRewardDataValue3(int _pos)
{
	//if( (int)m_rewardDataList.size() == 0 || _pos < (int)m_rewardDataList.size() || (int)m_rewardDataList.size() <= _pos )
	if( (int)m_rewardDataList.size() == 0 || _pos >= (int)m_rewardDataList.size() || _pos < 0 )
		return -1;

	return m_rewardDataList[_pos].value3;
}
