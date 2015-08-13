// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "Quest.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestProto::CQuestProto()
{
	m_index = 0;
	m_type[0] = -1;
	m_type[1] = -1;

	m_startType = -1;
	m_startData = 0;
	m_prizeNPC = -1;

	// 필요 조건
	m_needMinLevel = 1;
	m_needMaxLevel = 60;
	m_needJob = -1;
	memset(m_needItemIndex, -1, sizeof(int) * QUEST_MAX_NEED_ITEM);
	memset(m_needItemCount, 0, sizeof(int) * QUEST_MAX_NEED_ITEM);

	m_needRvRType = 0;
	m_needRvRGrade = 0;

	// 수행 조건
	memset(m_conditionType, -1, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionIndex, 0, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionNum, 0, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionData, 0, sizeof(m_conditionData));

	// 보상 조건
	memset(m_prizeType, -1, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_prizeIndex, 0, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_prizeData, 0, sizeof(int) * QUEST_MAX_PRIZE);

	// 보상 조건
	memset(m_optPrizeType, -1, sizeof(int) * QUEST_MAX_OPTPRIZE);
	memset(m_optPrizeIndex, 0, sizeof(int) * QUEST_MAX_OPTPRIZE);
	memset(m_optPrizeData, 0, sizeof(int) * QUEST_MAX_OPTPRIZE);

	m_prequestNum =0;
	m_startNpcZoneNum = 0;
	m_prizeNpcZoneNum = 0;
	m_needExp = 0;

	m_failValue = -1;
	m_partyScale = 0;

	memset(m_StartGiveItem, -1, sizeof(int) * QUESTITEM_MAX_ARRAYCOUNT);
	m_StartGiveKindCount = 0;
	memset(m_StartGiveNumCount, 0, sizeof(int) * QUESTITEM_MAX_ARRAYCOUNT);
}

//////////////////////////
// CQuestProtoList member

CQuestProtoList::CQuestProtoList()
{
	m_proto = NULL;
	m_nCount = 0;

	std::string tstr = "";
	for (int i = 0; i < QUEST_MAX_NEED_ITEM; ++i)
	{
		tstr = boost::str(boost::format("a_need_item%d") % i);
		a_need_item_str.push_back(tstr);

		tstr = boost::str(boost::format("a_need_item_count%d") % i);
		a_need_item_count_str.push_back(tstr);
	}

	for (int i = 0; i < QUEST_MAX_CONDITION; ++i)
	{
		tstr = boost::str(boost::format("a_condition%d_type") % i);
		a_condition_type_str.push_back(tstr);

		tstr = boost::str(boost::format("a_condition%d_index") % i);
		a_condition_index_str.push_back(tstr);

		tstr = boost::str(boost::format("a_condition%d_num") % i);
		a_condition_num_str.push_back(tstr);

		for (int j = 0; j < QUEST_MAX_CONDITION_DATA; ++j)
		{
			tstr = boost::str(boost::format("a_condition%d_data%d") % i % j);
			a_condition_data_str[i].push_back(tstr);
		}
	}

	for (int i = 0; i < QUEST_MAX_PRIZE; ++i)
	{
		tstr = boost::str(boost::format("a_prize_type%d") % i);
		a_prize_type_str.push_back(tstr);

		tstr = boost::str(boost::format("a_prize_index%d") % i);
		a_prize_index_str.push_back(tstr);

		tstr = boost::str(boost::format("a_prize_data%d") % i);
		a_prize_data_str.push_back(tstr);
	}

	for (int i = 0; i < QUEST_MAX_OPTPRIZE; ++i)
	{
		tstr = boost::str(boost::format("a_opt_prize_type%d") % i);
		a_opt_prize_type_str.push_back(tstr);

		tstr = boost::str(boost::format("a_opt_prize_index%d") % i);
		a_opt_prize_index_str.push_back(tstr);

		tstr = boost::str(boost::format("a_opt_prize_data%d") % i);
		a_opt_prize_data_str.push_back(tstr);
	}
}

CQuestProtoList::~CQuestProtoList()
{
	if (m_proto)
		delete [] m_proto;

	m_proto = NULL;
}

bool CQuestProtoList::Load()
{
	CDBCmd dbQuest;
	dbQuest.Init(&gserver->m_dbdata);
	dbQuest.SetQuery("SELECT * FROM t_quest WHERE a_enable=1 ORDER BY a_index");

	if (!dbQuest.Open())
		return false;

	m_nCount = dbQuest.m_nrecords;
	m_proto = new CQuestProto[m_nCount];

	int i, j, k;

	if(!dbQuest.MoveFirst())
		return true;

	for (i=0; i < m_nCount; i++)
	{
		dbQuest.GetRec("a_index",			m_proto[i].m_index);
		dbQuest.GetRec("a_type1",			m_proto[i].m_type[0]);
		dbQuest.GetRec("a_type2",			m_proto[i].m_type[1]);
		dbQuest.GetRec("a_start_type",		m_proto[i].m_startType);
		dbQuest.GetRec("a_start_data",		m_proto[i].m_startData);
		dbQuest.GetRec("a_prize_npc",		m_proto[i].m_prizeNPC);

		dbQuest.GetRec("a_prequest_num",		m_proto[i].m_prequestNum);
		dbQuest.GetRec("a_start_npc_zone_num",	m_proto[i].m_startNpcZoneNum);
		dbQuest.GetRec("a_prize_npc_zone_num",	m_proto[i].m_prizeNpcZoneNum);
		dbQuest.GetRec("a_need_exp",		m_proto[i].m_needExp);

		dbQuest.GetRec("a_need_min_level",	m_proto[i].m_needMinLevel);
		dbQuest.GetRec("a_need_max_level",	m_proto[i].m_needMaxLevel);
		dbQuest.GetRec("a_need_job",		m_proto[i].m_needJob);
		dbQuest.GetRec("a_need_rvr_type",	m_proto[i].m_needRvRType);
		dbQuest.GetRec("a_need_rvr_grade",	m_proto[i].m_needRvRGrade);

		dbQuest.GetRec("a_failvalue",		m_proto[i].m_failValue);
		dbQuest.GetRec("a_partyscale",		m_proto[i].m_partyScale);

		for (j=0; j < QUEST_MAX_NEED_ITEM; j++)
		{
			dbQuest.GetRec(a_need_item_str[j].c_str(), m_proto[i].m_needItemIndex[j]);
			dbQuest.GetRec(a_need_item_count_str[j].c_str(), m_proto[i].m_needItemCount[j]);
		}

		for (j=0; j < QUEST_MAX_CONDITION; j++)
		{
			dbQuest.GetRec(a_condition_type_str[j].c_str(), m_proto[i].m_conditionType[j]);
			dbQuest.GetRec(a_condition_index_str[j].c_str(), m_proto[i].m_conditionIndex[j]);
			dbQuest.GetRec(a_condition_num_str[j].c_str(),	m_proto[i].m_conditionNum[j]);

			for (k=0; k < QUEST_MAX_CONDITION_DATA; k++)
			{
				dbQuest.GetRec(a_condition_data_str[j][k].c_str(), m_proto[i].m_conditionData[j][k]);
			}
		}

		for (j=0; j < QUEST_MAX_PRIZE; j++)
		{
			dbQuest.GetRec(a_prize_type_str[j].c_str(), m_proto[i].m_prizeType[j]);
			dbQuest.GetRec(a_prize_index_str[j].c_str(), m_proto[i].m_prizeIndex[j]);
			dbQuest.GetRec(a_prize_data_str[j].c_str(), m_proto[i].m_prizeData[j]);
		}

		for (j=0; j < QUEST_MAX_OPTPRIZE; j++)
		{
			dbQuest.GetRec(a_opt_prize_type_str[j].c_str(), m_proto[i].m_optPrizeType[j]);
			dbQuest.GetRec(a_opt_prize_index_str[j].c_str(), m_proto[i].m_optPrizeIndex[j]);
			dbQuest.GetRec(a_opt_prize_data_str[j].c_str(), m_proto[i].m_optPrizeData[j]);
		}

		dbQuest.MoveNext();

		map_.insert(map_t::value_type(m_proto[i].m_index, &m_proto[i]));
	}

	return true;
}

CQuestProto* CQuestProtoList::FindProto(int index)
{
	map_t::iterator it = map_.find(index);
	return (it != map_.end()) ? it->second : NULL;
}

CQuestProto* CQuestProtoList::GetProtoIndexByNPC(int mobIndex )
{
	if( m_proto == NULL )
		return NULL;

	int i, j,  k;
	for(k=0; k<m_nCount; k++)
	{
		if( &m_proto[k] == NULL)
			continue;

		switch( m_proto[k].m_type[0] )
		{
		case QTYPE_KIND_REPEAT:
		case QTYPE_KIND_COLLECTION:
		case QTYPE_KIND_DEFEAT:
		case QTYPE_KIND_SAVE:
			break;
		default:
			continue;
		}

		for(i=0; i<QUEST_MAX_CONDITION; i++)
		{
			if ( m_proto[k].m_conditionData[i][0] <= 0)
				break;

			for(j=0; j<	QUEST_MAX_ITEM_DROP_MONSTER; j++)
			{
				if( m_proto[k].m_conditionData[i][j] <= 0 )
					break;

				if( m_proto[k].m_conditionData[i][j] == mobIndex )
					return &m_proto[k];
			}
		}
	}
	return NULL;
}

///////////////
// class CQuest
CQuest::CQuest(const CQuestProto* pQuestProto)
{
	m_pQuestProto = pQuestProto;
	m_cQuestState = QUEST_STATE_INIT;
	memset(m_nQuestValue, 0, sizeof(m_nQuestValue));
	m_bComplete1 = false;
	m_bComplete2 = false;

	m_pPrev = NULL;
	m_pNext = NULL;

	m_nFailValue = 0;
	m_nCompleteTime = 0;
}

CQuest::~CQuest()
{
	m_pQuestProto = NULL;
	m_cQuestState = QUEST_STATE_INIT;
	m_bComplete1 = false;
	m_bComplete2 = false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

void CQuest::QuestUpdateData(CPC* pPC, CNPC* pNPC)
{
	int i, j;

	// 혼자 npc를 다잡았어야 진행!! 구출 퀘스트는 제외
	// TODO : npc를 혼자 다 안잡아도 막타만 치면 되도록 한다!!!
	//if (npc->m_attackList->m_damage < npc->m_totalDamage && this->m_proto->m_type[0] != QTYPE_KIND_SAVE)
	//	return;

	// 이미 완료했으면 return
	if (IsCompleted())
		return ;

	// CONDITION에 따라 검사 처음에 모두 false로 셋팅
	// 조건 검사 하면서 만족하는 조건만 true로 셋팅
	bool bComplete[QUEST_MAX_CONDITION];
	memset(bComplete, 0, sizeof(bComplete));

	switch (GetQuestType0())
	{
	// 수집형 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
		{
			for (i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						bool bDropNpc = false;

						if( pNPC )
						{
							// condition 부가 데이타 중에 때린 pNPC가 있으면 해당 아이템 인벤으로
							for (j = 0; j < QUEST_MAX_ITEM_DROP_MONSTER; j++)
							{
								if (m_pQuestProto->m_conditionData[i][j] == pNPC->m_proto->m_index)
								{
									bDropNpc = true;
									break;
								}
							}
						}

						// 수행조건에 해당하는 pNPC를 때린것이 아님 리턴!
						if ( pNPC && !bDropNpc)
							continue;  //return;

						// 조건보다 이상이면 더이상 아이템 안줌
						if (GetQuestValue(i) >= m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}

						CItem* item = NULL;
						if (GetRandom(1, 10000) <= m_pQuestProto->m_conditionData[i][QUEST_MAX_CONDITION_DATA-1])
							item = gserver->m_itemProtoList.CreateItem(m_pQuestProto->m_conditionIndex[i], -1, 0, 0, 1);

						if (!item)
							continue;

						if (pPC->m_inventory.addItem(item) == false)
						{
							delete item;

							CNetMsg::SP rmsg(new CNetMsg);
							SysFullInventoryMsg(rmsg, 0);
							SEND_Q(rmsg, pPC->m_desc);

							return ;
						}

						// 이미 완료했으면 return
						if (IsCompleted())
							break;

						if( GetQuestValue(i) < m_pQuestProto->m_conditionNum[i] )
							IncreaseQuestValue(i);

						bComplete[i] = false;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							QuestStatusMsg(rmsg, this);
							SEND_Q(rmsg, pPC->m_desc);
						}

						// LOG
						GAMELOG << init("ITEM_PICK_QUESTITEM", pPC)
								<< itemlog(item)
								<< end;

						if (GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						else if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
							bComplete[i] = false;
						else
							continue;
					}
					break;
				}
			}
		}
		break;

	// 전달 퀘스트
	case QTYPE_KIND_DELIVERY:
		break;

	// 반복 퀘스트
	case QTYPE_KIND_REPEAT:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						if (m_pQuestProto->m_conditionIndex[i] == pNPC->m_proto->m_index)
						{
							if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
							{
								bComplete[i] = false;
								IncreaseQuestValue(i);

								CNetMsg::SP rmsg(new CNetMsg);
								QuestStatusMsg(rmsg, this);
								SEND_Q(rmsg, pPC->m_desc);
							}

							// 같으면 bComplete true 셋팅
							if (GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
							{
								bComplete[i] = true;
								continue;
							}
							else if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
								bComplete[i] = false;
							else
								continue;
						}
						else if(GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
					}
					break;
				}
			}
		}
		break;

	// 싱글던전2 구출 퀘스트
	case QTYPE_KIND_SAVE:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 공격한 몹이 죽었고 아이템 드롭 몬스터(보스)이면 아이템 지급
						if (DEAD(pNPC) && pNPC->m_proto->m_index == m_pQuestProto->m_conditionData[i][0])
						{
							CItem * item = gserver->m_itemProtoList.CreateItem(m_pQuestProto->m_conditionIndex[i], -1, 0, 0, 1);

							if (!item)
								continue;

							if (pPC->m_inventory.addItem(item) == false)
							{
								// 인젠토리 꽉차서 못 받을 때 Drop
								item = pNPC->m_pArea->DropItem(item, pNPC);
								if (!item)
									return;

								{
									CNetMsg::SP rmsg(new CNetMsg);
									item->m_preferenceIndex = pPC->m_index;
									ItemDropMsg(rmsg, pNPC, item);
									pNPC->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
								}
							}

							IncreaseQuestValue(i);

							// LOG
							GAMELOG << init("ITEM_PICK_QUESTITEM", pPC)
									<< itemlog(item)
									<< end;

							// 구출 퀘스트의 경우 그냥 리턴
							//return; //왜 그냥 리턴을 하지???
						}
					}
					break;
				}
			}
		}
		break;

	// 싱글던전1 격파 퀘스트
	case QTYPE_KIND_DEFEAT:
		{
			/////////////////////////////////////////
			//싱글 던젼 4 처리 : BW
			//
			if(this->m_pQuestProto->m_index == 105)
			{
				if(( pNPC->m_proto->m_index == 210
						|| pNPC->m_proto->m_index == 211
						|| pNPC->m_proto->m_index == 212
						|| pNPC->m_proto->m_index == 213
						|| pNPC->m_proto->m_index == 214 ) && DEAD(pNPC) )
				{
					IncreaseQuestValue(0);
					// 랭킹 디비 마리수 업데이트
				}

				if(m_pQuestProto->m_conditionNum[0] <= GetQuestValue(0))
				{
					for(i = 0; i < QUEST_MAX_CONDITION; i++)
						bComplete[i] = true;

					// 랭킹 디비 클리어/ 시간 업데이트
				}

				break;
			}

			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						// 공격하는 몹이 싱글던전 완료조건 몹의 인덱스와 같고 죽었으면
						if (m_pQuestProto->m_conditionIndex[i] == pNPC->m_proto->m_index && DEAD(pNPC))
						{
							bComplete[i] = true;
							IncreaseQuestValue(i);
//#ifdef GER_LOG
//							GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
//								<< LOG_VAL("Quest.cpp",":643") << blank
//							<< LOG_VAL("account-id", pPC->m_desc->m_idname ) << blank
//							<< LOG_VAL("character-id", pPC->m_desc->m_pChar->m_name ) << blank
//							<< LOG_VAL("quest-id", GetQuestIndex() ) << blank
//							<< LOG_VAL("zone-id", pPC->m_desc->m_pChar->m_pZone->m_index ) << blank
//							<< LOG_VAL("End-type", GetQuestProto()->m_prizeNPC ) << blank
//							<< LOG_VAL("npc-id", GetPrizeNPCIndex() ) << blank
//							<< LOG_VAL("item-id", GetQuestProto()->m_prizeIndex[i] ) << blank
//							<< LOG_VAL("amount", GetQuestProto()->m_prizeData[i] ) << blank
//							<< endGer;
//#endif //GER_LOG
						}
					}
					break;
				}
			}
		}
		break;

	default:
		return;
		break;
	}

	CheckComplete(pPC);

	// bComplete가 모두 true로 셋팅 되있어야 ㅇㅋ
	for (i=0; i < QUEST_MAX_CONDITION; i++)
	{
		if (!bComplete[i])
			return;
	}

	///////////////////////////////////
	// pd4 퀘스트이면 helper end : BW
	// MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	// char index를 보냄
	if(this->m_pQuestProto->m_index == 105)
	{
		GAMELOG << init("PD4 END", pPC)
				<< GetQuestValue(0) << end;

		if (gserver->isRunHelper())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_PD4_RANK_END
						 << pPC->m_index
						 << GetQuestValue(0)
						 << (char) 1;
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
					<< pPC->m_index << end;

			return;
		}
	}

	// Complete Msg 최초전송인경우에만 전송
	if (!IsCompleted())
	{
		if(GetComplete2())
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (QuestUpdateData)", pPC)
					<< m_pQuestProto->m_index
					<< end;
//#ifdef GER_LOG
//			for( int rewardItemCount = 0 ; rewardItemCount < 5 ; rewardItemCount++ )
//			{
//				GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
//					<< LOG_VAL("Quest.cpp",":723") << blank
//					<< LOG_VAL("account-id", pPC->m_desc->m_idname ) << blank
//					<< LOG_VAL("character-id", pPC->m_desc->m_pChar->m_name ) << blank
//					<< LOG_VAL("quest-id", GetQuestIndex() ) << blank
//					<< LOG_VAL("zone-id", pPC->m_desc->m_pChar->m_pZone->m_index ) << blank
//					<< LOG_VAL("End-type", GetQuestProto()->m_prizeNPC ) << blank
//					<< LOG_VAL("npc-id", GetPrizeNPCIndex() ) << blank
//					<< LOG_VAL("item-id", GetQuestProto()->m_prizeIndex[rewardItemCount] ) << blank
//					<< LOG_VAL("amount", GetQuestProto()->m_prizeData[rewardItemCount] ) << blank
//					<< endGer;
//			}
//#endif //GER_LOG
			// 완료 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCompleteMsg(rmsg, this);
			SEND_Q(rmsg, pPC->m_desc);
		}
		SetComplete1(true);
	}
}

void CQuest::QuestUpdateData( CPC* pPC, CPC* tPC )
{
	int i, j;

	if (IsCompleted())
		return ;

	// 조건 검사 하면서 만족하는 조건만 true로 셋팅
	bool bComplete[QUEST_MAX_CONDITION];
	memset(bComplete, 0, sizeof(bComplete));

	switch (GetQuestType0())
	{
	// 반복 퀘스트
	case QTYPE_KIND_REPEAT:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_PC:
					{
						if( m_pQuestProto->m_needRvRType > 0 && m_pQuestProto->m_needRvRType == tPC->getSyndicateType())
						{
							if(m_pQuestProto->m_needRvRGrade == 0)
							{
								if(m_pQuestProto->m_needRvRGrade > tPC->m_syndicateManager.getSyndicateGrade(tPC->getSyndicateType()))
									continue;
							}
							else
							{
								if(m_pQuestProto->m_needRvRGrade != tPC->m_syndicateManager.getSyndicateGrade(tPC->getSyndicateType()))
									continue;
							}
						}

						if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = false;
							IncreaseQuestValue(i);

							CNetMsg::SP rmsg(new CNetMsg);
							QuestStatusMsg(rmsg, this);
							SEND_Q(rmsg, pPC->m_desc);
						}
						// 같으면 bComplete true 셋팅
						if (GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						else if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
							bComplete[i] = false;
						else
							continue;
					}
					break;
				}
			}
		}
		break;
	default:
		return;
		break;
	}

	CheckComplete(pPC);

	// bComplete가 모두 true로 셋팅 되있어야 ㅇㅋ
	for (i=0; i < QUEST_MAX_CONDITION; i++)
	{
		if (!bComplete[i])
			return;
	}

	// Complete Msg 최초전송인경우에만 전송
	if (!IsCompleted())
	{
		if(GetComplete2())
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (QuestUpdateData)", pPC)
					<< m_pQuestProto->m_index
					<< end;
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCompleteMsg(rmsg, this);
			SEND_Q(rmsg, pPC->m_desc);
		}
		SetComplete1(true);
	}
}

void CQuest::RemoeQuestItem(CPC* pc)
{
	for (int k = 0; k < QUEST_MAX_CONDITION; k++)
	{
		// 수행조건 아이템을 찾고
		if (m_pQuestProto->m_conditionType[k] != QCONDITION_ITEM)
			continue;

		item_search_t vec;
		int search_count = pc->m_inventory.searchItemByCondition(m_pQuestProto->m_conditionIndex[k], 0, 0, vec);
		if (search_count == 0 || search_count < m_pQuestProto->m_conditionNum[k])
			continue ;

		pc->m_inventory.deleteItem(vec, m_pQuestProto->m_conditionNum[k]);
	}
}

///////////////////
// class CQuestList
CQuestList::CQuestList()
{
	m_head = NULL;
	m_nCountRun = 0;
	m_nCountDone = 0;
	m_nCountAbandon = 0;
	m_nCountComplete = 0;
}

CQuestList::~CQuestList()
{
	clear();
}

CQuest* CQuestList::AddQuest(int nQuestIndex)
{
	const CQuestProto* pQuestProto = gserver->m_questProtoList.FindProto(nQuestIndex);
	if (pQuestProto == NULL)
		return NULL;
	CQuest* pQuest = new CQuest(pQuestProto);
	pQuest->SetNextQuest(m_head);
	if (m_head)
		m_head->SetPrevQuest(pQuest);
	m_head = pQuest;
	return pQuest;
}

void CQuestList::DelQuest(CPC* pPC, int questIndex)
{
	CQuest* pDelQuest;
	CQuest* pDelQuestNext = GetNextQuest(NULL, QUEST_STATE_ALL);
	while ((pDelQuest = pDelQuestNext))
	{
		pDelQuestNext = GetNextQuest(pDelQuestNext, QUEST_STATE_ALL);
		if (pDelQuest->m_pQuestProto->m_index == questIndex)
		{
			CQuest* pPrevQuest = pDelQuest->GetPrevQuest();
			CQuest* pNextQuest = pDelQuest->GetNextQuest();
			if (pPrevQuest)
				pPrevQuest->SetNextQuest(pNextQuest);
			if (pNextQuest)
				pNextQuest->SetPrevQuest(pPrevQuest);
			if (pDelQuest == m_head)
				m_head = m_head->GetNextQuest();

			if(pDelQuest->GetQuestState() == QUEST_STATE_RUN)
				m_nCountRun--;
			else if(pDelQuest->GetQuestState() == QUEST_STATE_DONE)
				m_nCountDone--;
			else if(pDelQuest->GetQuestState() == QUEST_STATE_ABANDON)
				m_nCountAbandon--;
			return;
		}
	}
}

bool CQuestList::DelQuest(CPC* pPC, CQuest* pQuest, char cQuestState)
{
	CQuest* pDelQuest;
	CQuest* pDelQuestNext = GetNextQuest(NULL, cQuestState);
	while ((pDelQuest = pDelQuestNext))
	{
		pDelQuestNext = GetNextQuest(pDelQuestNext, cQuestState);
		if (pDelQuest == pQuest)
		{
			CQuest* pPrevQuest = pDelQuest->GetPrevQuest();
			CQuest* pNextQuest = pDelQuest->GetNextQuest();
			if (pPrevQuest)
				pPrevQuest->SetNextQuest(pNextQuest);
			if (pNextQuest)
				pNextQuest->SetPrevQuest(pPrevQuest);
			if (pDelQuest == m_head)
				m_head = m_head->GetNextQuest();

			if(cQuestState == QUEST_STATE_RUN)
				m_nCountRun--;
			else if(cQuestState == QUEST_STATE_DONE)
				m_nCountDone--;
			else if(cQuestState == QUEST_STATE_ABANDON)
				m_nCountAbandon--;
			return true;
		}
	}
	return false;
}

void CQuestList::DelQuestALL(CPC* pPC, char cQuestState)
{
	CQuest* pDelQuest;
	CQuest* pDelQuestNext = GetNextQuest(NULL, cQuestState);
	while ((pDelQuest = pDelQuestNext))
	{
		pDelQuestNext = GetNextQuest(pDelQuestNext, cQuestState);
		CQuest* pPrevQuest = pDelQuest->GetPrevQuest();
		CQuest* pNextQuest = pDelQuest->GetNextQuest();
		if (pPrevQuest)
			pPrevQuest->SetNextQuest(pNextQuest);
		if (pNextQuest)
			pNextQuest->SetPrevQuest(pPrevQuest);
		if (pDelQuest == m_head)
			m_head = m_head->GetNextQuest();

		if(cQuestState == QUEST_STATE_RUN)
			m_nCountRun--;
		else if(cQuestState == QUEST_STATE_DONE)
			m_nCountDone--;
		else if(cQuestState == QUEST_STATE_ABANDON)
			m_nCountAbandon--;
	}
}

CQuest* CQuestList::FindQuest(int nQuestIndex)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		if (pFindQuest->GetQuestIndex() == nQuestIndex)
			return pFindQuest;
	}
	return NULL;
}

bool CQuestList::FindQuest(int nQuestIndex, char cQuestState )
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, cQuestState );
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, cQuestState );
		if (pFindQuest->GetQuestIndex() == nQuestIndex)
			return true;
	}
	return false;
}

CQuest* CQuestList::FindQuestByItem(int nitemIndex)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		for(int i=0; i<QUEST_MAX_CONDITION; i++)
		{
			if (pFindQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_USE )
				if(pFindQuest->GetQuestProto()->m_conditionIndex[i] == nitemIndex)
					return pFindQuest;
		}
	}
	return NULL;
}

CQuest* CQuestList::FindQuestByMob(int nMobIndex)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		for(int i=0; i<QUEST_MAX_CONDITION; i++)
		{
			if (pFindQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_NPC )
				if(pFindQuest->GetQuestProto()->m_conditionIndex[i] == nMobIndex)
					return pFindQuest;
		}
	}
	return NULL;
}

//  [2/23/2007 KwonYongDae]
// 퀘스트 타입 0 이 진행중 이면 true
bool CQuestList::IsQuestType0( int qType0 )
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		if ( pFindQuest->GetQuestType0() == qType0 && !pFindQuest->IsCompleted() )
			return true;
	}
	return false;
}

void CQuestList::SetQuestState(CQuest* pQuest, char cQuestState)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext);
		if (pFindQuest == pQuest)
		{
			switch (pFindQuest->GetQuestState())
			{
			case QUEST_STATE_RUN:
				m_nCountRun--;
				break;
			case QUEST_STATE_DONE:
				m_nCountDone--;
				break;
			case QUEST_STATE_ABANDON:
				m_nCountAbandon--;
				break;
			default:
				break;
			}
			pFindQuest->SetQuestState(cQuestState);
			switch (pFindQuest->GetQuestState())
			{
			case QUEST_STATE_RUN:
				m_nCountRun++;
				break;
			case QUEST_STATE_DONE:
				m_nCountDone++;
				break;
			case QUEST_STATE_ABANDON:
				m_nCountAbandon++;
				break;
			default:
				break;
			}
			return ;
		}
	}
}

CQuest* CQuestList::GetNextQuest(CQuest* pCurrentQuest, char cQuestState)
{
	if (pCurrentQuest == NULL)
		pCurrentQuest = m_head;
	else
		pCurrentQuest = pCurrentQuest->GetNextQuest();
	while (pCurrentQuest)
	{
		if (cQuestState == QUEST_STATE_ALL || pCurrentQuest->GetQuestState() == cQuestState)
			return pCurrentQuest;
		pCurrentQuest = pCurrentQuest->GetNextQuest();
	}
	return NULL;
}

void CQuestList::CheckComplete(CPC* pc)
{
	CQuest* pQuestNext;
	CQuest* pQuest;
	pQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while( (pQuest = pQuestNext) )
	{
		pQuestNext = GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		pQuest->CheckComplete(pc);
	}
}

void CQuestList::clear()
{
	while (m_head)
	{
		CQuest*		pQuest = m_head;
		m_head = m_head->GetNextQuest();
		delete pQuest;
	}
	m_nCountRun = 0;
	m_nCountDone = 0;
	m_nCountAbandon = 0;
	m_nCountComplete = 0;
}

bool CQuestList::isRvrQuest(CPC* pc, int questIndex)
{
	if (questIndex == 682)
	{
		// 해당 퀘스트를 가지고 있거나 해당 퀘스트를 완료했거나
		// 683 퀘스트를 완료 했거나
		if (FindQuest(questIndex, QUEST_STATE_DONE) || FindQuest(questIndex))
			return true;
		else if (FindQuest(683, QUEST_STATE_DONE) || FindQuest(683))
			return true;
	}
	else if (questIndex == 683)
	{
		// 해당 퀘스트를 가지고 있거나 해당 퀘스트를 완료했거나
		// 682 퀘스틀 완료했거나 가지고 있거나
		if (FindQuest(questIndex, QUEST_STATE_DONE) || FindQuest(questIndex))
			return true;
		else if (FindQuest(682, QUEST_STATE_DONE) || FindQuest(682))
			return true;
	}
	return false;
}

void CQuest::QuestUpdateDataForParty(CPC* pPC, CNPC* pNPC)
{
	if( GetPartyScale() == QTYPE_SCALE_PARTY && pPC->IsParty() )
	{
		int i;
		const CPartyMember* pPartyMember;
		CPC* pPartyPC;
		CQuest* quest;
		for(i = 0; i <MAX_PARTY_MEMBER; ++i)
		{
			pPartyMember = pPC->m_party->GetMemberByListIndex(i);
			if(pPartyMember && pPartyMember->GetMemberPCPtr())
			{
				pPartyPC = pPartyMember->GetMemberPCPtr();

				if(pPC->m_pArea->FindCharInCell(pPC, pPartyPC->m_index, MSG_CHAR_PC))
				{
					quest = pPartyPC->m_questList.FindQuest(GetQuestIndex());
					if(quest && quest->GetQuestState() == QUEST_STATE_RUN)
						quest->QuestUpdateData(pPartyPC, pNPC);
				}
			}
		}
	}
#ifdef _BATTLEGROUP_QUEST_BUG_PIX
	else if( GetPartyScale() == QTYPE_SCALE_BATTLEGROUP && pPC->IsExped() )
	{
		int i,j;
		CPC* pExpedPC;
		CQuest* quest;

		for (i=0; i < MAX_EXPED_GROUP; i++)
		{
			for (j=0; j < MAX_EXPED_GMEMBER; j++)
			{
				pExpedPC = pPC->m_Exped->GetMemberPCPtr(i,j);
				if (pExpedPC == NULL)
					continue;

				if(pPC->m_pArea->FindCharInCell(pPC, pExpedPC->m_index, MSG_CHAR_PC))
				{
					quest = pExpedPC->m_questList.FindQuest(GetQuestIndex());
					if(quest && quest->GetQuestState() == QUEST_STATE_RUN)
						quest->QuestUpdateData(pExpedPC, pNPC);
				}
			}
		}
	}
#endif
	else
		QuestUpdateData(pPC, pNPC);
}

void CQuest::QuestUpdateDataForParty(CPC* pPC, CNPC* pNPC, int questIndex)
{
}

bool CQuest::CheckComplete(CPC * pc)
{
	int i;

	switch ( GetQuestType0() )
	{
	case QTYPE_KIND_REPEAT:
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
	case QTYPE_KIND_PROCESS_EXPERIENCE :		//  [2/22/2007 KwonYongDae] QuestCheck
	case QTYPE_KIND_MAKE_EXPERIENCE :
		{
			bool complete = true;
			bool complete2 = true;

			for ( i=0; i<QUEST_MAX_CONDITION; i++)
			{
				if (GetQuestProto()->m_conditionType[i] != QCONDITION_ITEM_NORMAL)
					continue;

				if (GetQuestValue(i) >= GetQuestProto()->m_conditionNum[i])
					continue;

				complete2 = false;
				break;
			}

			for (i=0; i<QUEST_MAX_CONDITION; i++)
			{
				//if (pQuestProto->m_conditionType[j] != QCONDITION_ITEM)
				if (GetQuestProto()->m_conditionType[i] == -1 || GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_NORMAL)
					continue;

				if (GetQuestValue(i) >= GetQuestProto()->m_conditionNum[i])
					continue;

				complete = false;
				break;
			}

//- 이것 때문에 두번째 퀘스트 조건이 완료 되지 않아도 퀘스트가 완료된 처리르 한다.
			if(complete2)
			{
				SetComplete2(true);
			}

			if (complete)
			{
				if(complete2)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuestCompleteMsg(rmsg, this);
					SEND_Q(rmsg, pc->m_desc);
				}
				SetComplete1(true);
			}
			else
				SetComplete1(false);
		}
		break;

	case QTYPE_KIND_DELIVERY:
		{
			bool bcomplete = true;

			if(  GetQuestProto()->m_index == 370 )
			{
				if(pc->m_teachIdx[0] > 0)
				{
					bool bcomplete = true;
				}
				else
					break;
			}

			for ( i = 0; i< QUEST_MAX_CONDITION; i++)
			{
				if (GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM )
				{
					item_search_t vec;
					int search_count = pc->m_inventory.searchItemByCondition(GetQuestProto()->m_conditionIndex[i], 0, 0, vec);
					if (search_count == 0 || search_count < GetQuestProto()->m_conditionNum[i])
					{
						bcomplete = false;
						break;
					}
				}
			}

#ifdef QUEST_EXTEND_TH
			if(bcomplete)
			{
				SetComplete(0, true);
				SetComplete(1, true);

				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, this);
				SEND_Q(rmsg, pc->m_desc);
			}
#else
			if(bcomplete)
			{
				SetComplete1(true);
				SetComplete2(true);

				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, this);
				SEND_Q(rmsg, pc->m_desc);
			}
#endif
		}
		break;

	case QTYPE_KIND_DEFEAT :
	case QTYPE_KIND_SAVE :
		{
			bool complete = true;
			for ( i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (GetQuestProto()->m_conditionType[i] != QCONDITION_ITEM && GetQuestProto()->m_conditionType[i] != QCONDITION_NPC)
					continue;

				if (GetQuestValue(i) >= GetQuestProto()->m_conditionNum[i])
					continue;

				complete = false;
				break;
			}

			if(complete)
			{
#ifdef QUEST_EXTEND_TH
				SetComplete(0, true);			//싱글던전 퀘스트이므로..
				SetComplete(1, true);
#else
				SetComplete1(true);
				SetComplete2(true);
#endif
				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, this);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	case QTYPE_KIND_SEARCH: // 탐색 퀘스트 (특정 위치까지 갔다오는 ...)
		{
			bool complete = true;

			for (i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if (GetQuestProto()->m_conditionType[i] == -1 )
					continue;

				if (GetQuestProto()->m_conditionType[i] != QCONDITION_AREA)
				{
					complete =false;
					break;
				}

				if(GetQuestValue(i) >= GetQuestProto()->m_conditionNum[i])
					continue;
				else
				{
					complete = false;
					break;
				}
			}

			if(complete)
			{
#ifdef QUEST_EXTEND_TH
				SetComplete(0, true);
				SetComplete(1, true);
#else
				SetComplete1(true);
				SetComplete2(true);
#endif
				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, this);
				SEND_Q(rmsg, pc->m_desc);
			}
		}

	default:
		{
			bool complete	= true;
			bool complete2	= true;
			bool complete3	= true;

			if( GetQuestProto()->m_conditionType[0] != -1 )
			{
				if( GetQuestValue(0) < GetQuestProto()->m_conditionNum[0] )
					complete = false;
			}

			if( GetQuestProto()->m_conditionType[1] != -1 )
			{
				if( GetQuestValue(1) < GetQuestProto()->m_conditionNum[1] )
					complete2 = false;
			}

			if( GetQuestProto()->m_conditionType[2] != -1 )
			{
				if( GetQuestValue(2) < GetQuestProto()->m_conditionNum[2] )
					complete2 = false;
			}

			if( complete && complete2 && complete3 )
			{
				SetComplete1(true);
				SetComplete2(true);

				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, this);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	}
	return false;
}
