#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "DropItem.h"

bool CDropManager::loadDropData(MYSQL * _dataDB)
{
	CDBCmd cmdHead;
	cmdHead.Init(_dataDB);

	std::string sql = "SELECT * FROM t_drop_item_head ORDER BY a_drop_idx";

	cmdHead.SetQuery(sql);

	if( !cmdHead.Open() || !cmdHead.MoveFirst() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "NO EXIST TABLE DROP ITEM HEAD" << end;
		return false;\
	}

	if( cmdHead.m_nrecords < 1 )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "EMPTY TABLE DROP ITEM HEAD" << end;
		return false;
	}

	CDBCmd cmdData;
	cmdData.Init(_dataDB);

	int dropIndex=0, dropType=0, levelmini=0, levelMaxi=0, dropProb=0, probPerLevel=0, party=1, boss=1;
	CLCString npclist;
	CLCString levelsection(256);
	do
	{
		if(    !cmdHead.GetRec("a_drop_idx",		dropIndex)
				|| !cmdHead.GetRec("a_drop_type",		dropType)
				|| !cmdHead.GetRec("a_level_mini",		levelmini)
				|| !cmdHead.GetRec("a_level_maxi",		levelMaxi)
				|| !cmdHead.GetRec("a_prob",			dropProb)
				|| !cmdHead.GetRec("a_prob_level",		probPerLevel)
				|| !cmdHead.GetRec("a_npc_type_party",	party)
				|| !cmdHead.GetRec("a_npc_type_boss",	boss)
				|| !cmdHead.GetRec("a_npc",				npclist)
				|| !cmdHead.GetRec("a_level_section",	levelsection)
		  )
		{
			GAMELOG << init("EVENT_AUTOMATION_WARN") << "NOT MATCHING FIELD IN TABLE DROP ITEM HEAD" << end;
			return false;
		}

		std::string select_drop_item_data_sql = boost::str(boost::format("SELECT * FROM t_drop_item_data WHERE a_drop_idx=%d ") % dropIndex);
		cmdData.SetQuery( select_drop_item_data_sql );

		if( !cmdData.Open()  )
		{
			GAMELOG << init("EVENT_AUTOMATION_WARN") << "NO EXIST TABLE DROP ITEM DATA" << end;
			return false;
		}

		if( !cmdData.MoveFirst() )
			continue;

		if( cmdData.m_nrecords < 1 )
			continue;

		CDropInfo dData(dropIndex, dropType, levelmini, levelMaxi, dropProb, probPerLevel, party, boss);

		const char* pArg = npclist;
		char buf[8]= {0,};
		int npcIndex=0;
		while (*pArg)
		{
			pArg = AnyOneArg(pArg, buf);
			npcIndex = atoi(buf);
			if( npcIndex > 0 )
				dData.addNpc( npcIndex);
		}

		pArg = levelsection;
		int lvlstart, lvlend, lvlprob;
		while (*pArg)
		{
			pArg = AnyOneArg(pArg, buf);
			lvlstart	= atoi(buf);
			pArg = AnyOneArg(pArg, buf);
			lvlend		= atoi(buf);
			pArg = AnyOneArg(pArg, buf);
			lvlprob		= atoi(buf);

			if( lvlprob > 0 )
			{
				CProbByLevelSection ls(lvlstart, lvlend, lvlprob);
				dData.addLevelSection( ls );
			}
		}

		int index=0, plus=0, flag=0, prob=0, jobFlag =0;
		LONGLONG count=0;

		do
		{
			if(	   !cmdData.GetRec("a_item_idx",	index)
					|| !cmdData.GetRec("a_plus",		plus)
					|| !cmdData.GetRec("a_flag",		flag)
					|| !cmdData.GetRec("a_count",		count)
					|| !cmdData.GetRec("a_prob",		prob)
					|| !cmdData.GetRec("a_job_flag",	jobFlag)
			  )
			{
				GAMELOG << init("EVENT_AUTOMATION_WARN") << "NOT MATCHING FIELD IN TABLE DROP ITEM DATA" << end;
				return false;
			}

			CDropItemData diData(index, plus, flag, count, prob, jobFlag );
			dData.addItem( diData );
		}
		while( cmdData.MoveNext() );

		m_dataList.insert( std::map<int,CDropInfo>::value_type( dropIndex, dData ) );
	}
	while( cmdHead.MoveNext() );

	return true;
}

bool CDropManager::doDrop(CPC* _pPC, CNPC * _pNPC, const int _dropIndex, const int _prob, const int _pos, bool _bPreferenceIndex)
{
	if( _pPC == NULL || _pNPC == NULL )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "_pPC, _pNPC is 0" << end;
		return false;
	}

	CDropInfo * pDrop = getDropInfo(_dropIndex);
	if( pDrop == NULL )
		return false;

	if( !checkCondition( _pPC, _pNPC, pDrop) )
		return false;

	switch( pDrop->getDropType() )
	{
	case 0: // Multi 확률만 맞는 아이템은 다 떨어 뜨린다.
		return doDropRandom(_pPC, _pNPC, pDrop, _bPreferenceIndex);
	case 1: // Once-Random 리스트중에 확률에 따라 하나만 떨구나.
		return doDropOnceRandom(_pPC, _pNPC, pDrop, _prob, _bPreferenceIndex);
	case 2: // Once-Select 리스트중에 하나를 선택해 드랍 // 외부 선택으로 돌릴까나? ㅡㅡ;
		return doDropOnceSelect(_pPC, _pNPC, pDrop, _pos, _bPreferenceIndex);
	default:
		return false;
	}

	return true;
}

int CDropManager::getDropItemListCount(const int _dropIndex)
{
	CDropInfo * pDrop = getDropInfo(_dropIndex);
	if( pDrop == NULL )
		return 0;

	return pDrop->getItemCount();
}

CDropInfo * CDropManager::getDropInfo( const int _dropIndex)
{
	std::map<int,CDropInfo>::iterator itr;
	itr = m_dataList.find( _dropIndex );

	if( itr == m_dataList.end() )
		return NULL;

	return &itr->second;
}
bool CDropManager::checkCondition(CPC* _pPC, CNPC* _pNPC, CDropInfo* _pDrop)
{
	if( !checkNpc(_pDrop, _pNPC->m_idNum) )
		return false;

	if( !checkLevelLimit(_pDrop, _pPC->m_level, _pNPC->m_level) )
		return false;

	if( !checkDropProb(_pNPC, _pDrop) )
		return false;

	return true;
}

bool CDropManager::checkNpc(CDropInfo* _pDrop, const int _npcDBIndex)
{
	if( _pDrop->getNpcCount() == 0 )
		return true;

	int i=0;
	int npcCount = _pDrop->getNpcCount();
	for(i=0; i<npcCount; i++)
	{
		if( _pDrop->getNpcIndex(i) == _npcDBIndex)
			return true;
	}

	return false;
}

bool CDropManager::checkLevelLimit(CDropInfo* _pDrop, const int _charlevel, const int _npcLevel)
{
	if( _pDrop->getLevelMini() != -1 && _charlevel-_npcLevel > _pDrop->getLevelMini() )
		return false;

	if( _pDrop->getLevelMaxi() != -1 && _npcLevel-_charlevel > _pDrop->getLevelMaxi() )
		return false;

	return true;
}

bool CDropManager::checkDropProb(CNPC* _pNPC, CDropInfo* _pDrop)
{
	if( _pDrop->getProb() == -1 )
		return true;

	if(  getDropProb(_pNPC, _pDrop) >= getRandom() )
		return true;

	return false;
}

int CDropManager::getDropProb(CNPC* _pNPC, CDropInfo* _pDrop)
{
	int prob = _pDrop->getProb();

	if( _pNPC->m_proto->CheckFlag( NPC_BOSS ) )
		prob += _pDrop->getProb() * _pDrop->getNpcTypeBoss();
	else if( _pNPC->m_proto->CheckFlag( NPC_PARTY ) )
		prob += _pDrop->getProb() * _pDrop->getNpcTypeBoss();

	prob += getProbByLevelSection( _pDrop, _pNPC->m_level );

	if( prob > 10000 )
		prob = 10000;

	return prob;
}

int CDropManager::getProbByLevelSection( CDropInfo* _pDrop, const int _npcLevel)
{
	if( _pDrop->getLevelSectionCount() == 0 )
		return 0;

	int i=0, prob = 0;
	int levelSectionCount = _pDrop->getLevelSectionCount();
	for(i=0; i<levelSectionCount; i++)
	{
		if(  _pDrop->getLevelSection(i)->getStart() <= _npcLevel && _pDrop->getLevelSection(i)->getEnd() >= _npcLevel )
		{
			prob = _pDrop->getLevelSection(i)->getProb();
			break;
		}
	}

	if( prob < 0 )
		prob = 0;

	return prob;
}

bool CDropManager::doDropRandom(CPC* _pPC, CNPC* _pNPC , CDropInfo* _pDrop, bool _bPreferenceIndex)
{
	if( _pDrop->getItemCount() == 0 )
		return false;

	int i=0;

	int itemCount = _pDrop->getItemCount();
	for(i=0; i<itemCount; i++ )
	{
		if( !checkJob(_pDrop->getItemData(i)->getJobFlag(), _pPC->m_job ) )
			continue;

		if( !checkProbPerLevel(_pDrop->getItemData(i)->getProb(), _pPC->m_level-_pNPC->m_level, _pDrop->getProbPerLevel()) )
			continue;

		if( !dropItem(_pPC, _pNPC, _pDrop->getItemData(i), _bPreferenceIndex) )
			continue;
	}

	return true;
}

bool CDropManager::doDropOnceRandom(CPC* _pPC, CNPC* _pNPC , CDropInfo* _pDrop,const int _prob, bool _bPreferenceIndex)
{
	if( _pDrop->getItemCount() == 0 )
		return false;

	int probPerLevel = getProbPerLevel( _pPC->m_level-_pNPC->m_level, _pDrop->getProbPerLevel() );
	int prob = -1;

	if( _prob < 0 || _prob > 10000 )
		prob = getRandom() - probPerLevel;
	else
		prob = _prob - probPerLevel;

	if( prob < 0 )
		return false;

	int totalProb = 0;

	int i=0;
	int itemCount = _pDrop->getItemCount();
	for(i=0; i<itemCount; i++ )
	{
		totalProb += _pDrop->getItemData(i)->getProb();

//		if( !checkJob(_pDrop->getItemData(i)->getJobFlag(), _pPC->m_job ) )
//			continue;

		if( prob > totalProb )
			continue;

		if( !dropItem(_pPC, _pNPC, _pDrop->getItemData(i), _bPreferenceIndex) )
			return false;

		return true;
	}
	return true;
}

bool CDropManager::doDropOnceSelect(CPC* _pPC, CNPC* _pNPC , CDropInfo* _pDrop, const int _pos, bool _bPreferenceIndex)
{
	if( _pDrop->getItemCount() == 0 )
		return false;

	int pos = -1;

	if( _pos < 0 || _pos >= _pDrop->getItemCount() )
		pos = GetRandom(0, _pDrop->getItemCount()-1);
	else
		pos = _pos;

//		if( !checkJob(_pDrop->getItemData(i)->getJobFlag(), _pPC->m_job ) )
//			return false;

	if( !dropItem(_pPC, _pNPC, _pDrop->getItemData(pos), _bPreferenceIndex) )
		return false;

	return true;
}

bool CDropManager::checkProbPerLevel(const int _prob, const int _levelDiff, const int _probPerLevel)
{
	if( _prob == 10000 )
		return true;

	if( _prob - getProbPerLevel(_levelDiff,_probPerLevel) >= getRandom() )
		return true;

	return false;
}

int CDropManager::getProbPerLevel(const int _levelDiff, const int _probPerLevel)
{
	if( _probPerLevel == 0 || _levelDiff < 0 )
		return 0;

	return (_levelDiff * _probPerLevel);
}

bool CDropManager::checkJob( const int _jobFlag, const int _charJob )
{
	if( _jobFlag & ( 1<< _charJob) )
		return true;

	return false;
}
bool CDropManager::dropItem(CPC* _pPC, CNPC* _pNPC, CDropItemData * _pDropItem, bool _bPreferenceIndex)
{
	CItem* pItem = gserver->m_itemProtoList.CreateItem(
					   _pDropItem->getIndex(), -1 , _pDropItem->getPlus(), _pDropItem->getFlag(), _pDropItem->getCount() );

	if (!pItem)
	{
		GAMELOG << init("DROP_SYSTEM ITEM DROP FAILD", _pPC)
				<< _pDropItem->getIndex()	<< delim
				<< _pDropItem->getPlus()		<< delim
				<< _pDropItem->getFlag()		<< delim
				<< _pDropItem->getCount()
				<< end;
		return false;
	}

	if( _bPreferenceIndex )
		pItem->m_preferenceIndex = _pPC->m_index;

	if( _pDropItem && _pDropItem->getIndex() == NAS_ITEM_DB_INDEX)
	{
		//돈일 경우 돈 메시지 전송
		GoldType_t moneyCount = _pDropItem->getCount();

		_pPC->m_inventory.increaseMoney(moneyCount);
		GAMELOG << init("DROP_SYSTEM ITEM NAS INPUT SUCC", _pPC) << moneyCount << end;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		_pNPC->m_pArea->DropItem(pItem, (CCharacter*)_pNPC);
		ItemDropMsg(rmsg, (CCharacter*)_pNPC, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
		GAMELOG << init("DROP_SYSTEM ITEM DROP SUCC", _pPC) << itemlog(pItem) << end;
	}
	return true;
}
