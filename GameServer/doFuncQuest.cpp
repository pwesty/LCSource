#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_old_do_sskill.h"

///////////////////////
// Quest 관련 함수

void getRvrQuest(CNPC* npc, int* questIndex)
{
	int kailuxJoinCount = SyndicateInfoDataManager::instance()->getJoinCount(SYNDICATE::eSYNDICATE_KAILUX);
	int dealermoonJoinCount = SyndicateInfoDataManager::instance()->getJoinCount(SYNDICATE::eSYNDICATE_DEALERMOON);
	int totalJoinCount = kailuxJoinCount + dealermoonJoinCount;
	int kailuxJoinPercent		= 0;
	int dealermoonJoinPercent	= 0;
	if (totalJoinCount != 0)
	{
		if (0 < kailuxJoinCount)
		{
			kailuxJoinPercent		= kailuxJoinCount / totalJoinCount * 1000;
		}
		if (0 < dealermoonJoinCount)
		{
			dealermoonJoinPercent	= dealermoonJoinCount / totalJoinCount * 1000;
		}
	}

	if (npc->m_proto->m_index == 1540)
	{
		if( totalJoinCount > 100)
		{
			if (kailuxJoinPercent < dealermoonJoinPercent)
			{
				if ((dealermoonJoinPercent - kailuxJoinPercent) > SYDICATE_QUEST_LIST_PERCENT)
				{
					questIndex[0] = 682;
					return;
				}
			}
			else if (dealermoonJoinPercent < kailuxJoinPercent)
			{
				if ((kailuxJoinPercent - dealermoonJoinPercent) > SYDICATE_QUEST_LIST_PERCENT)
				{
					questIndex[0] = 683;
					return;
				}
			}
		}

		questIndex[0] = 682;
		questIndex[1] = 683;
	}
}


void do_Quest(CPC* ch, CNetMsg::SP& msg)
{
	if (DEAD(ch))
		return ;

	msg->MoveFirst();

	unsigned char subtype;

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_QUEST_REQUEST:
		do_QuestRequest(ch, msg);
		break;

	case MSG_QUEST_START:
		do_QuestStart(ch, msg);
		break;

	case MSG_QUEST_GIVEUP:
		do_QuestGiveUp(ch, msg);
		break;

	case MSG_QUEST_PRIZE:
		do_QuestPrize(ch, msg);
		break;

	case MSG_QUEST_COMPLETE:
		do_QuestComplete(ch, msg);
		break;
	// BW

	case MSG_QUEST_PD4_RANK_VIEW_REQ:			// 랭크 뷰요청
		do_QuestPD4RankView(ch, msg);
		break;
	case MSG_QUEST_PD4_RANK_REWARD_RANK_REQ:	// 보상 뷰요청
		do_QuestPD4RankRewardView(ch, msg);
		break;
	case MSG_QUEST_PD4_REWARD:					// 보상 요청
		do_QuestPD4RankReward(ch, msg);
		break;

	case MSG_QUEST_COLLECT:
		do_QuestCollect(ch, msg);				// 수집 퀘스트
		break;

	case MSG_QUEST_RESTORE_ABANDON:
		do_QuestRestoreAbandon(ch, msg);		// 포기 퀘스트 복구
		break;
	case MSG_QUEST_ITEM_REQ:
		do_QuestItemReq(ch, msg);				// 완료한 퀘스트중 퀘스트 아이템 받기
		break;
	}
}

void do_QuestPD4RankView(CPC* ch, CNetMsg::SP& msg)
{
	/////////////
	// helper에게 랭크 뷰 요청 : BW
	//MSG_HELPER_PD4_RANK_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	char charjob;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> charjob;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RankView FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RankView FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> charjob;
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
#ifdef NPC_CHECK
		RefMsg(rmsg) << MSG_HELPER_PD4_RANK_VIEW_REQ
					 << ch->m_index
					 << charjob
					 << npcIndex;
#else
		RefMsg(rmsg) << MSG_HELPER_PD4_RANK_VIEW_REQ
					 << ch->m_index
					 << charjob;
#endif

		if (gserver->isRunHelper())
		{
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			GAMELOG << init("PD4RANKVIEW REQ FAIL", ch) << end;
		}
	}
}
void do_QuestPD4RankRewardView(CPC* ch, CNetMsg::SP& msg)
{
	//////////////////////////
	//헬퍼에게 보상뷰 요청
	//MSG_HELPER_PD4_REWARD_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c), charname(str)
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RankRewardView FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RankRewardView FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
#ifdef NPC_CHECK
		RefMsg(rmsg) << MSG_HELPER_PD4_REWARD_VIEW_REQ
					 << ch->m_index
					 << ch->m_job
					 << npcIndex;
#else
		RefMsg(rmsg) << MSG_HELPER_PD4_REWARD_VIEW_REQ
					 << ch->m_index
					 << ch->m_job;
#endif
		if (gserver->isRunHelper())
		{
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			GAMELOG << init("PD4REWARDVIEW REQ FAIL", ch) << end;
		}
	}
}
void do_QuestPD4RankReward(CPC* ch, CNetMsg::SP& msg)
{
	//////////////////////////////////
	// 헬퍼에게 보상 받을 사람 요청
	// 보상 안해주면 존내 맞는거다 얼씨구
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RankReward FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("RankReward FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_PD4_REWARD_REQ
					 << ch->m_index;

		if (gserver->isRunHelper())
		{
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			GAMELOG << init("PD4REWARD REQ FAIL", ch) << end;
		}
	}
}

void do_QuestRequest(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	int npcIndex;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
				>> npcIndex;

	// 해당 Quest npc가 주위에 있는가
	CCharacter* tch = NULL;

	CNPC* npc = TO_NPC(ch->m_pArea->FindProtoNPCInCell( (CCharacter*)ch, npcIndex, false));

	// 주위에 해당 npc 없음
	if (!npc)
		return;

	// Quest Request Log
//#ifdef GER_LOG
//	GAMELOGGEM << init( 0, "CHAR_QUEST_REQUEST")
//		<< LOG_VAL("account-id", ch->m_desc->m_idname) << blank
//		<< LOG_VAL("char-id", ch->m_desc->m_pChar->m_name) << blank
//		<< LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
//		<< LOG_VAL("npc-id", npcIndex ) << blank
//		<< endGer;
//#endif //GER_LOG
	GAMELOG << init("QUEST REQUEST",  ch)
			<< npcIndex
			<< end;

	// npc가 보여줄 최대 퀘스트 수
	int	count = npc-> m_nQuestCount+ QUEST_MAX_PC;

	// 퀘스트 플래그
	int* flag;
	flag = new int [count];
	memset(flag, -1, sizeof(int) * count);

	// 퀘스트 인덱스
	int* index;
	index = new int [count];
	memset(index, -1, sizeof(int) * count);

	// 퀘스트 보상의 경우
	int i;
	int questCount = 0;

	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if (	pQuest->IsCompleted()
				&&	pQuest->GetPrizeNPCIndex() == npc->m_proto->m_index
				&&	(pQuest->GetPrizeNPCZone() == -1 || pQuest->GetPrizeNPCZone() == npc->m_pZone->m_index)
		   )
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_COMPLETE;
			index[questCount] = pQuest->GetQuestIndex();
			questCount++;
		}
	}

	// 퀘스트를 받으려는 경우
	int rvrQuestInex[2] = {0,};
	getRvrQuest(npc, rvrQuestInex);
	for (i=0; i < npc->m_nQuestCount; i++)
	{
		int j;
		int questIndex = 0;
		if (npc->m_proto->m_index != 1540)
		{
			questIndex = npc->m_quest[i];
		}
		else if(ch->isSyndicate() == false)
		{
			questIndex = rvrQuestInex[i];
		}

		if (questIndex == 682 || questIndex == 683)
		{
			if (ch->m_questList.isRvrQuest(ch, questIndex))
			{
				continue;
			}
		}

		CQuestProto* proto = gserver->m_questProtoList.FindProto(questIndex);

		if (!proto)
			continue;

		if(proto->m_startNpcZoneNum != -1 && (proto->m_startNpcZoneNum != npc->m_pZone->m_index))
		{
			continue;
		}

		CQuest* pQuest;

		if (proto->m_type[1] == QTYPE_REPEAT_ONCE || proto->m_type[1] == QTYPE_REPEAT_DAY) // 수행한 퀘스트는 리스트에서 제외
		{
			bool bOnce = false;		// 이미 수행한 퀘스트 인가

			CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext);
				switch (pQuest->GetQuestState())
				{
				case QUEST_STATE_DONE:
				case QUEST_STATE_ABANDON:
					if (pQuest->GetQuestIndex() == proto->m_index)
						bOnce = true;
					break;
				default:
					break;
				}
				if (bOnce)
					break;;
			}
			if (bOnce)
				continue;
		}

		// 수행중인가
		pQuest = ch->m_questList.FindQuest(proto->m_index);
		// 수행중이면서
		if (pQuest && pQuest->GetQuestState() == QUEST_STATE_RUN)
		{
			// 완료면 안됨
			if (pQuest->IsCompleted())
				continue;
			// 완료 아니면
			else
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_ING;
				index[questCount] = proto->m_index;
				questCount++;
				continue;
			}
		}

		// 필요 레벨 조건
		if (proto->m_needMinLevel > ch->m_level || proto->m_needMaxLevel < ch->m_level)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_LEVEL;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// 경험치가 부족하다..
		/*if((proto->m_needMinLevel == ch->m_level) && proto->m_needExp != 0 &&	(ch->m_exp < proto->m_needExp) )
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_EXP;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}*/

		//선행퀘스트를 수행하였는가...
		if(proto->m_prequestNum != 0)
		{
			bool bOnce = false;

			CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_DONE);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_DONE);
				if (pQuest->GetQuestIndex() == proto->m_prequestNum)
				{
					bOnce = true;
					break;
				}
			}

			if (!bOnce)//아직 선행 퀘스트를 완료하지 않았다면,
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOT_PREQUEST;
				index[questCount] = proto->m_index;
				questCount++;
				continue;
			}
		}

		// 필요 직업 조건
		if (proto->m_needJob != -1 && proto->m_needJob != ch->m_job)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_JOB;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// rvr 퀘스트일 때 필요조건
		if (proto->m_needRvRType > 0)
		{
			if(	proto->m_needRvRType != ch->getSyndicateType() ||
					proto->m_needRvRGrade > ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType()) )
			{
				continue;
			}
		}

		// 필요 아이템 체크
		int k;
		bool bNeedItem = true;	// 필요아이템 조건 만족

		for (j=0; j < QUEST_MAX_NEED_ITEM; j++)
		{
			if (proto->m_needItemIndex[j] == -1)
				continue;

			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(proto->m_needItemIndex[j], 0, 0, vec);
			if (sc == 0 || sc < proto->m_needItemCount[j])
			{
				bNeedItem = false;
				break;
			}
		}

		if (!bNeedItem)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_ITEM;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// 격파, 구출 퀘스트의 경우 싱글던전 여유분 체크
		if (proto->m_type[0] == QTYPE_KIND_DEFEAT || proto->m_type[0] == QTYPE_KIND_SAVE)
		{
			int zoneidx = -1;

			switch (proto->m_index)
			{
			case 13:		// 발록 격파
				zoneidx = ZONE_SINGLE_DUNGEON1;
				break;
			case 14:		// 공주 구출
				zoneidx = ZONE_SINGLE_DUNGEON2;
				break;
			case 104:		// 아자카 협곡
				// 하루 3회 까지만
				if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON3))
				{
					flag[questCount] = (int)MSG_QUEST_FLAG_OVERCOUNT;
					index[questCount] = proto->m_index;
					questCount++;
					continue ;
				}
				zoneidx = ZONE_SINGLE_DUNGEON3;
				break;
			case 105:		// 차원의 문
				// 하루 3회 까지만
				if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON4))
				{
					flag[questCount] = (int)MSG_QUEST_FLAG_OVERCOUNT;
					index[questCount] = proto->m_index;
					questCount++;

					continue ;
				}
				zoneidx = ZONE_SINGLE_DUNGEON4;
				break;
//#endif
			}

			CZone* pZone = gserver->FindZone(zoneidx);
			if (pZone == NULL)
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOTENOUGH_AREA;
				index[questCount] = proto->m_index;
				questCount++;
				continue ;
			}

			for (j=0 ; j < pZone->m_countArea; j++)
			{
				if (!pZone->m_area[j].m_bEnable)
					break;
			}
			// 싱글던전 모두 꽉차다 목록에 나오지 않도록 continue;
			if (j == pZone->m_countArea)
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOTENOUGH_AREA;
				index[questCount] = proto->m_index;
				questCount++;
				continue ;
			}
		}

		// 하드코딩 : 생산스킬습득 퀘스트 새로 나오지 않도록 조정
//		if (proto->m_index == 27 || proto->m_index == 28 || proto->m_index == 29)
//			continue;

		// 초기값 그대로라면 수행 가능
		if (flag[questCount] == -1)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_CAN;
			index[questCount] = proto->m_index;
			questCount++;
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestNPCListMsg(rmsg, questCount, flag, index);
		SEND_Q(rmsg, ch->m_desc);
	}

	if (flag)
		delete [] flag;
	if (index)
		delete [] index;
}

bool do_QuestStart(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	int questIndex;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
				>> questIndex;

	// 10개 초과
	if (ch->m_questList.GetCountRun() >= QUEST_MAX_PC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_MAX);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	CQuestProto* proto = gserver->m_questProtoList.FindProto(questIndex);

	if (!proto)
		return false;

	// 시작 조건
	if (proto->m_startType == QSTART_LEVEL)
	{
		if (proto->m_startData > ch->m_level)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_LEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}
	else if(proto->m_startType == QSTART_NPC)
	{
		// NPC와의 셀 2칸
		CNPC* npc = ch->m_pArea->FindProtoNPCInCell(ch, proto->m_startData, false, 2);
		if (npc == NULL)
		{
			LOG_INFO("not found npc. char_index[%d] npcIndex[%d]", ch->m_index, proto->m_startData);
			return false;
		}

		if (proto->m_startNpcZoneNum != ch->m_pArea->m_zone->m_index )
		{
			LOG_INFO("not equal npc index. char_index[%d] npcIndex[%d]", ch->m_index, proto->m_startData);
			return false;
		}
	}
	else if(proto->m_startType == QSTART_ITEM)
	{
		if (ch->m_inventory.FindByDBIndex(proto->m_startData, 0, 0) == NULL)
		{
			GAMELOG << init("HACK USER BY QUEST", ch) << "QUEST START TYPE" << delim << proto->m_startType << delim << "QUEST INDEX" << delim << questIndex << end;
			return false;
		}
	}
	else
		return false;

	// 1회용 퀘스트 이면서 실행했는지
	CQuest* pQuest;
	CQuest* pQuestNext;
#if defined (LC_GAMIGO) || defined (QUEST_EXTEND_REPEAT_DAY)
	if (proto->m_type[1] == QTYPE_REPEAT_ONCE || proto->m_type[1] == QTYPE_REPEAT_DAY)
#else
	if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
#endif // defined (LC_GAMIGO)
	{
		pQuestNext = ch->m_questList.GetNextQuest(NULL);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext);
			if (pQuest->GetQuestIndex() == proto->m_index)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_ONCE);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}
	}

	// 수행중인가
	pQuest = ch->m_questList.FindQuest(proto->m_index);
	if (pQuest)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 필요 레벨 조건
	if (proto->m_needMinLevel > ch->m_level || proto->m_needMaxLevel < ch->m_level)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 필요 직업 조건
	if (proto->m_needJob != -1 && proto->m_needJob != ch->m_job)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_JOB);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// rvr 퀘스트일 때 필요조건
	if (proto->m_needRvRType > 0)
	{
		if(	proto->m_needRvRType != ch->getSyndicateType() ||
				proto->m_needRvRGrade > ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType()) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_JOB);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}

	// 격파, 구출 퀘스트의 경우 싱글던전 여유분 체크
	// 입장 제한
	if (proto->m_type[0] == QTYPE_KIND_DEFEAT || proto->m_type[0] == QTYPE_KIND_SAVE)
	{
		int zoneidx = -1;
		switch (proto->m_index)
		{
		case 13:		// 발록 격파
			zoneidx = ZONE_SINGLE_DUNGEON1;
			break;
		case 14:		// 공주 구출
			zoneidx = ZONE_SINGLE_DUNGEON2;
			break;
		case 104:		// 아자카 협곡
			// 하루 3회 까지만
			if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON3))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_MAX);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}

			zoneidx = ZONE_SINGLE_DUNGEON3;
			break;
		case 105:		// 차원의 문
			// 하루 3회 까지만
			if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON4))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_MAX);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}

			ch->m_pd4StartTime = TIME_ONE_HOUR; //1시간

			zoneidx = ZONE_SINGLE_DUNGEON4;
			break;
//#endif
		}

		CZone* pZone = gserver->FindZone(zoneidx);
		if (pZone == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_FULL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}

		int i = 0;
		for (; i < pZone->m_countArea; i++)
		{
			if (!pZone->m_area[i].m_bEnable)
				break;
		}
		// 싱글던전 모두 꽉차다
		if (i == pZone->m_countArea)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_FULL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}

	// 필요 아이템 체크
	item_search_t delete_vec[QUEST_MAX_NEED_ITEM];
	for (int i = 0; i < QUEST_MAX_NEED_ITEM; i++)
	{
		if (proto->m_needItemIndex[i] == -1)
			continue;

		int sc = ch->m_inventory.searchItemByCondition(proto->m_needItemIndex[i], 0, 0, delete_vec[i]);
		if (sc == 0 || sc < proto->m_needItemCount[i])
		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_START_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}

	{
		// 전달 퀘스트일경우 전달할 아이템을 받을 공간이 있는지 미리 검사
		if (proto->m_type[0] == QTYPE_KIND_DELIVERY && questIndex != 153)
		{
			int empty_count = 0;
			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if (proto->m_conditionType[i] == -1)
					continue;


				switch (proto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
				case QCONDITION_ITEM_NORMAL:
					++empty_count;
					break;
				}
			}

			if (ch->m_inventory.getEmptyCount() < empty_count)
			{
				// 인젠토리 꽉차서 못 받을 때
				GAMELOG << init("QUEST ERROR", ch)
						<< proto->m_index << delim
						<< (int)MSG_QUEST_ERR_DELIVERY_FULL
						<< end;

				CNetMsg::SP rmsg(new CNetMsg);
				QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}
	}

	pQuest = ch->m_questList.AddQuest(proto->m_index);
	if (pQuest == NULL)
		return false;

	// 아이템 수량 변경
	for (int i = 0; i < QUEST_MAX_NEED_ITEM; i++)
	{
		if (proto->m_needItemIndex[i] == -1)
			continue;

		ch->m_inventory.deleteItem(delete_vec[i], proto->m_needItemCount[i]);
	}

	ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
	pQuest->SetComplete1(false);
	pQuest->SetComplete2(false);

	///////////////////////////////////
	// pd4 퀘스트이면 helper 랭킹에 등록 : BW
	//
	if (proto->m_index == 105)
	{
		GAMELOG << init("PD4 START", ch) << end;

		if (gserver->isRunHelper())
		{
			// char index를 보냄
			CNetMsg::SP rmsg(new CNetMsg);

			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_PD4_RANK_ADD
						 << ch->m_index;
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			GAMELOG << init("PD4 QUEST ERROR : RANK ADD ", ch) << end;
		}
	}

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_QUEST_STARTED")
			   << LOG_VAL("account-id", ch->m_desc->m_idname) << blank
			   << LOG_VAL("char-id", ch->m_desc->m_pChar->m_name) << blank
			   << LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
			   //<< LOG_VAL("npc-id", npcIndex ) << blank
			   << LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
			   << endGer;
#endif // GERLOG
	// Quest Start Log
	GAMELOG << init("QUEST START", ch)
			<< pQuest->GetQuestIndex()
			<< end;

	{
		// Quest Start Msg
		CNetMsg::SP rmsg(new CNetMsg);
		QuestStartMsg(rmsg, pQuest);
		SEND_Q(rmsg, ch->m_desc);
	}


	// 퀘스트 시작 시 뭔가 하는 것들
	switch (proto->m_type[0])
	{
	// 전달 퀘스트
	case QTYPE_KIND_DELIVERY:
		{
			//  [2/28/2007 KwonYongDae] 153번 퀘스트 인경우 아이템 지급 금지 응급 처치
			if( questIndex == 153 )
			{
				ch->m_questList.CheckComplete( ch );		// 퀘스트 완료 체크
				break;
			}

			// rvr 퀘스트 바로 완료 처리
			if (questIndex == 682 || questIndex == 683)
			{
				ch->m_questList.CheckComplete(ch);
				break;
			}

			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if (proto->m_conditionType[i] == -1)
					continue;

				switch (proto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
				case QCONDITION_ITEM_NORMAL:
					{
						CItem* item = gserver->m_itemProtoList.CreateItem(proto->m_conditionIndex[i], -1, 0, 0, proto->m_conditionNum[i]);
						if (!item)
							continue;

						if (ch->m_inventory.addItem(item) == false)
						{
							// 인젠토리 꽉차서 못 받을 때
							GAMELOG << init("QUEST ERROR", ch)
									<< proto->m_index << delim
									<< (int)MSG_QUEST_ERR_DELIVERY_FULL
									<< end;

							CNetMsg::SP rmsg(new CNetMsg);
							QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rmsg, ch->m_desc);

							delete item;

							return false;
						}

						// Item LOG
						GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
								<< itemlog(item)
								<< end;

						pQuest->SetQuestValue(i, proto->m_conditionNum[i]);
						if(pQuest->IsCompleted() == false)
						{
							for (int j = 0; j < QUEST_MAX_CONDITION; j++)
							{
								CItem* p = ch->m_inventory.FindByDBIndex(pQuest->GetQuestProto()->m_conditionIndex[j], 0, 0);
								if (p)
								{
									CheckQuestCondition(ch, p, p->Count());
								}
							}
						}
					}
					break;

				default:
					break;
				}
			}

			//ch->m_questList.SetComplete(idx, true);
			pQuest->SetComplete2(true);

			if( pQuest->GetQuestProto()->m_index == 370 )
			{
				// 후견인과 관계를 맺은 상태라면 바로 완료 메시지를 날려준다.
				if( ch->m_teachType == MSG_TEACH_STUDENT_TYPE
//					|| ch->m_teachType == MSG_TEACH_NO_STUDENT_TYPE  // 한번이라도 사제 관계를 맺었던 견습생도 퀘스트 완료를 해주려면 주석을 풀어버리삼
				  )
				{
					pQuest->SetComplete1(true);
					pQuest->SetComplete2(true);

					CNetMsg::SP rmsg(new CNetMsg);
					QuestCompleteMsg(rmsg, pQuest);
					SEND_Q(rmsg, ch->m_desc);
				}
				return true; // 리턴하진 않으면 바로 완료된다.
			}

			// Complete Msg 최초전송인경우에만 전송
			if (!pQuest->IsCompleted()	)
			{
				if (pQuest->GetComplete2())
				{
					// Quest Complete Log
					GAMELOG << init("QUEST COMPLETE (do_QuestStart)", ch)
							<< pQuest->GetQuestIndex()
							<< end;

					// 완료 메시지
					CNetMsg::SP rmsg(new CNetMsg);
					QuestCompleteMsg(rmsg, pQuest);
					SEND_Q(rmsg, ch->m_desc);
				}
				pQuest->SetComplete1(true);
			}
		}
		break;

	// 채굴, 채집, 차지 체험 퀘스트
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
		{
			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				CItem* p = ch->m_inventory.FindByDBIndex(pQuest->GetQuestProto()->m_conditionIndex[i], 0, 0);
				if (p)
				{
					CheckQuestCondition(ch, p, p->Count());
				}
			}

			// 생산도구 지급
			CItem* tool = NULL;

			for (int i = 0; i < gserver->m_itemProtoList.m_nCount; i++)
			{
				if (gserver->m_itemProtoList.m_protoItems[i].getItemTypeIdx() != ITYPE_WEAPON)
					continue;
				if (!(gserver->m_itemProtoList.m_protoItems[i].getItemJobFlag() & (1 << ch->m_job)))
					continue;

				switch(proto->m_type[0])
				{
				case QTYPE_KIND_MINING_EXPERIENCE:
					if (gserver->m_itemProtoList.m_protoItems[i].getItemSubTypeIdx() != IWEAPON_MINING)
						continue;
					break;

				case QTYPE_KIND_GATHERING_EXPERIENCE:
					if (gserver->m_itemProtoList.m_protoItems[i].getItemSubTypeIdx() != IWEAPON_GATHERING)
						continue;
					break;

				case QTYPE_KIND_CHARGE_EXPERIENCE:
					if (gserver->m_itemProtoList.m_protoItems[i].getItemSubTypeIdx() != IWEAPON_CHARGE)
						continue;
					break;
				}

				tool = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_protoItems[i].getItemIndex(), -1, 0, 0, 1);
				break;
			}

			if (!tool)
				return false;

			if (ch->m_inventory.addItem(tool) == false)
			{
				// Drop Msg 보내기
				tool = ch->m_pArea->DropItem(tool, ch);
				if (!tool)
					return false;

				CNetMsg::SP rmsg(new CNetMsg);
				tool->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, tool);
				ch->m_pArea->SendToCell(rmsg, GET_YLAYER(tool), tool->m_cellX, tool->m_cellZ);
			}

			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(tool)
					<< end;

			pQuest->SetComplete2(true);
		}
		break;

	// 시작시 아이템을 제공하는 퀘스트
	case QTYPE_KIND_PROCESS_EXPERIENCE :
	case QTYPE_KIND_MAKE_EXPERIENCE :
		{
			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_NORMAL)
				{
					CItem* p = ch->m_inventory.FindByDBIndex(pQuest->GetQuestProto()->m_conditionIndex[i],	0, 0);
					if (p)
					{
						CheckQuestCondition(ch, p, p->Count());
					}
				}
			}

			// QUEST 150 : 구리 가공 확률만 100%
			CItem * itemOfferd = NULL ;
			int		iOfferdIndex = -1;
			//int		itemOfferdIndex = -1;
			switch ( questIndex )
			{
			case 150 :
				iOfferdIndex = 269;
				break;		// 스톤 정제 체험 - 스톤온 정련서
			case 151 :
				iOfferdIndex = 279;
				break;		// 원소 정제 체험 - E등급 원소 정제서
			case 152 :
				iOfferdIndex = 274;
				break;		// 식물 가공 체험 - 크락의 노란잎 가공서
			case 154 :
				iOfferdIndex = 397;
				break;		// 무기 제작 체험 - 25레벨 무기제작서
			default :
				iOfferdIndex = -1;
			}
			if( iOfferdIndex > 0 )
				itemOfferd = gserver->m_itemProtoList.CreateItem( iOfferdIndex, -1, 0, 0, 1 );

			if( !itemOfferd )
				return false;

			if (ch->m_inventory.addItem(itemOfferd) == false)
			{
				itemOfferd = ch->m_pArea->DropItem( itemOfferd, ch );

				if( !itemOfferd )
					return false;

				// Drop Msg 보내기
				CNetMsg::SP rmsg(new CNetMsg);
				itemOfferd->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, itemOfferd);
				ch->m_pArea->SendToCell(rmsg, GET_YLAYER(itemOfferd), itemOfferd->m_cellX, itemOfferd->m_cellZ);
			}

			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(itemOfferd)
					<< end;

			pQuest->SetComplete2(true);
		}
		break;

	case QTYPE_KIND_REPEAT:
	case QTYPE_KIND_COLLECTION:
		{
			int bQuestItem, bNormalItem, bNpc, bpc;
			bQuestItem = bNormalItem = bNpc = bpc = 0;
			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuest->GetQuestProto()->m_conditionType[i] == -1)
					continue;

				if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM)
				{
					bQuestItem++;
				}
			}

			if(bQuestItem==0)
			{
				for (int i = 0; i < QUEST_MAX_CONDITION; i++)
				{
					if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_NPC)
					{
						bNpc++;
					}

					if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_PC)
					{
						bpc++;
					}
				}

				if(bNpc==0 && bpc==0)
				{
					pQuest->SetComplete1(true);
				}
				//ch->m_questList.SetComplete(idx, true);
			}

			for (int i = 0; i < QUEST_MAX_CONDITION; i++)
			{
				if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_NORMAL)
				{
					CItem* p = ch->m_inventory.FindByDBIndex(pQuest->GetQuestProto()->m_conditionIndex[i], 0, 0);
					if (p)
					{
						CheckQuestCondition(ch, p, p->Count());
					}

					bNormalItem++;
				}
			}

			if(bNormalItem==0)
			{
				//	pQuest->SetComplete2(true); // 정체가 뭐냐? 왜 트루 해주냐?  [7/31/2009 derek]
			}
		}
		break;
	case QTYPE_KIND_DEFEAT :
	case QTYPE_KIND_SAVE :
		pQuest->SetComplete2(true);
		break;
	default:
		break;
	}

	return true;
}

void do_QuestGiveUp(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	int questIndex;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
				>> questIndex;

	CQuest* pQuest;

	pQuest = ch->m_questList.FindQuest(questIndex);

	// 없는 퀘스트
	if (pQuest == NULL)
	{
		CQuestProto* proto = gserver->m_questProtoList.FindProto(questIndex);

		if(!proto)
			return;

		if(proto->m_type[1] == QTYPE_REPEAT_ONCE)
		{
			// Quest GiveUp Log
			GAMELOG << init("QUEST GIVEUP", ch)
					<< questIndex
					<< end;

			pQuest = ch->m_questList.AddQuest(questIndex);
			if (pQuest)
				ch->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestGiveUpMsg(rmsg, questIndex);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}

	if (pQuest->GetQuestState() != QUEST_STATE_RUN)
		return;

	if (pQuest->GetQuestProto()->m_index == 684 ||
			pQuest->GetQuestProto()->m_index == 685)
	{
		return;
	}

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();


	int i;

	switch (pQuestProto->m_type[0])
	{
	// 수집, 전달, 구출 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_DELIVERY:
	case QTYPE_KIND_SAVE:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
	case QTYPE_KIND_PROCESS_EXPERIENCE :
	case QTYPE_KIND_MAKE_EXPERIENCE :
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 퀘스트 인벤토리에서만
						CItem* item = ch->m_inventory.FindByDBIndex(pQuestProto->m_conditionIndex[i], 0, 0);
						if (item == NULL)
							continue;

						int nValue = pQuest->GetQuestValue(i);
						ch->m_inventory.decreaseItemCount(item, nValue);
					}
					break;
				default:
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	bool bGoZone = false;

	bool bNewTutorial = false;

	if (ch->m_pZone->IsPersonalDungeon())
	{
		if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
		{
			bGoZone = true;
		}
		else if(pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL)
		{
			bGoZone = true;
			bNewTutorial = true;
			ch->m_newtutoComplete = 0;
		}
	}

	// Quest GiveUp Log
	GAMELOG << init("QUEST GIVEUP", ch)
			<< pQuestProto->m_index
			<< end;

	// 여기서 포기한 퀘스트 인덱스를 저장하여 다음에는 받지 못하도록 한다.
	// 포기한 퀘스트를 저장한다.
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
	}
#if defined (QUEST_EXTEND_REPEAT_DAY)
	else if(pQuestProto->m_type[1] == QTYPE_REPEAT_DAY)
	{
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);

		tm tmNext = NOW();
		tmNext.tm_mday = tmNext.tm_mday + 1;
		tmNext.tm_hour = 0;
		tmNext.tm_min = 0;
		tmNext.tm_sec = 0;
		tmNext.tm_isdst = -1;

		pQuest->SetCompleteTime(mktime(&tmNext));
	}
#endif // QUEST_EXTEND_REPEAT_DAY


	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestGiveUpMsg(rmsg, pQuest);
		ch->m_questList.DelQuest(ch, pQuest);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 격파, 구출 테스트의 경우 존이동
	if (bGoZone)
	{
		// 가까운 마을로
		int nearZone;
		int nearZonePos;
		CZone* pZone = gserver->FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &nearZone, &nearZonePos);
		if (pZone == NULL)
			return;

		// 처음 들어온 캐릭터는 쥬노 입구에 보내준다.
		if(bNewTutorial)
		{
			if(pZone->m_countZonePos > 6)
			{
				GoZone(ch, nearZone,
					   pZone->m_zonePos[6][0],													// ylayer
					   GetRandom(pZone->m_zonePos[6][1], pZone->m_zonePos[6][3]) / 2.0f,		// x
					   GetRandom(pZone->m_zonePos[6][2], pZone->m_zonePos[6][4]) / 2.0f);		// z
			}
			else
			{
				GoZone(ch, nearZone,
					   pZone->m_zonePos[0][0],													// ylayer
					   GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
					   GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);		// z
			}
		}
		else
			GoZone(ch, nearZone,
				   pZone->m_zonePos[nearZonePos][0],															// ylayer
				   GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z
	}

	// 포기했으면 싱글던전 입장권 받을 기회 줄인다. 최소 0
	ch->m_questList.DecreaseQuestComepleteCount();
}

void do_QuestPrize(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	int questIndex, npcIndex;
	int optItemIndex;
	int plus;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
				>> questIndex
				>> npcIndex;

	RefMsg(msg) >> optItemIndex;
	RefMsg(msg) >> plus;

	CQuest* pQuest = ch->m_questList.FindQuest(questIndex);
	if (pQuest == NULL || pQuest->GetQuestState() != QUEST_STATE_RUN)
		return ;

	if (!pQuest->IsCompleted())
		return ;

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();


	// pQuest 보상을 받을 npc가 맞는지
	if (pQuestProto->m_prizeNPC != npcIndex)
		return;

	if( pQuestProto->m_prizeNPC > 0 )
	{
		CNPC* npc = ch->m_pArea->FindProtoNPCInCell(ch, pQuestProto->m_prizeNPC, false, 2);
		if (npc == NULL)
		{
			LOG_INFO("not found npc. char_index[%d] npcIndex[%d]", ch->m_index, pQuestProto->m_prizeNPC);
			return;
		}

		if (pQuestProto->m_prizeNpcZoneNum != ch->m_pArea->m_zone->m_index)
		{
			LOG_INFO("not equal npc index. char_index[%d] npcIndex[%d]", ch->m_index, pQuestProto->m_prizeNPC);
			return;
		}
	}

	// rvr 가입 보상일 경우
	bool bSyndicate = ch->isSyndicate();
	if (questIndex == 684 && npcIndex == 1541)
	{
		// 카이룩스 가입
		if (!bSyndicate)
			ch->m_syndicateManager.joinSyndicate(SYNDICATE::eSYNDICATE_KAILUX);
	}
	else if (questIndex == 685 && npcIndex == 1542)
	{
		// 딜라문 가입
		if (!bSyndicate)
			ch->m_syndicateManager.joinSyndicate(SYNDICATE::eSYNDICATE_DEALERMOON);
	}

	//아이템을 보상받는 퀘스트 일 경우

	for( int i=0; i<QUEST_MAX_PRIZE; i++)
	{
		if (pQuestProto->m_prizeType[i] == QPRIZE_ITEM)
		{
			if(ch->m_inventory.getEmptyCount() <= 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
				SEND_Q( rmsg, ch->m_desc);
				return ;
			}
		}
	}


	// [101214: selo] 보상 아이템 드롭 수정
	if( !ch->CheckInvenForQuestPrize(pQuestProto, optItemIndex) )
	{
		return;
	}

	int i;

	switch (pQuestProto->m_type[0])
	{
	// 전달 퀘스트
	// 수집 퀘스트
	// 구출 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_DELIVERY:
	case QTYPE_KIND_SAVE:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
	case QTYPE_KIND_PROCESS_EXPERIENCE :
	case QTYPE_KIND_MAKE_EXPERIENCE :
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				// 아예 없으면
				if (pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						item_search_t vec;
						int search_count = ch->m_inventory.searchItemByCondition(pQuestProto->m_conditionIndex[i], 0, 0, vec);
						if (search_count == 0 || search_count < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
									<< pQuestProto->m_index
									<< end;

							CNetMsg::SP rmsg(new CNetMsg);
							QuestFailMsg(rmsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}

						// Item 수량 변경
						ch->m_inventory.deleteItem(vec, pQuestProto->m_conditionNum[i]);
					}
					break;

				case QCONDITION_ITEM_NORMAL:
					{
						// 펫 아이템 체크.. (펫을 가지고 있다면 보상을 받지 않고 무시한다. return
						if( ch->m_petList )
						{
							int petCount = 0;
							CPet* p = ch->m_petList;
							while (p)
							{
								petCount++;
								p = p->m_nextPet;
							}

							if( (pQuest->IsPetQuest() ) && petCount >= MAX_OWN_PET )
							{
								CNetMsg::SP rmsg(new CNetMsg);
								QuestErrorMsg(rmsg, MSG_QUEST_ERR_PET_NOT_HAVE_ONEMORE);
								SEND_Q(rmsg, ch->m_desc);
								return;
							}
						}

						//일반아이템 체크.
						item_search_t vec;
						int sc = ch->m_inventory.searchItemByCondition(pQuestProto->m_conditionIndex[i], 0, 0, vec);
						if (sc == 0 || sc < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
									<< pQuestProto->m_index
									<< end;

							CNetMsg::SP rmsg(new CNetMsg);
							QuestFailMsg(rmsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}

						// Item 수량 변경
						ch->m_inventory.deleteItem(vec, pQuestProto->m_conditionNum[i]);
					}
					break;

				default:
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	for (i=0; i < QUEST_MAX_PRIZE; i++)
	{
		if (pQuestProto->m_prizeType[i] == -1)
			continue;

		switch (pQuestProto->m_prizeType[i])
		{
		case QPRIZE_ITEM:
			// 보상 아이템 주기
			{
				CItem* item = gserver->m_itemProtoList.CreateItem(pQuestProto->m_prizeIndex[i], -1, 0, 0, pQuestProto->m_prizeData[i]);

				if (!item)
					continue;

#ifdef SYSTEM_MONSTER_MERCENARY_CARD_LOW_LEVEL_SUPPORT
				if( pQuestProto->m_index==494 && item->getDBIndex()==6805 )	// 초보지원용 몬스터 용병 카드 아이템
				{
					int npcIndex =-1;
					switch( ch->m_job )
					{
					case JOB_TITAN:
						npcIndex=950;
						break;	// 형제단 기술자
					case JOB_KNIGHT:
						npcIndex=901;
						break;	// 하얀 루트
					case JOB_HEALER:
						npcIndex=920;
						break;	// 꿈꾸는 테라
					case JOB_MAGE:
						npcIndex=478;
						break;	// 아누비스 주술사
					case JOB_ROGUE:
						npcIndex=953;
						break;	// 형제단 재규어
					case JOB_SORCERER:
						npcIndex=919;
						break;	// 핏빛 흰 나무 정령
#ifdef EX_ROGUE
					case JOB_EX_ROGUE:
						npcIndex=953;
						break;	// 형제단 재규어
#endif // EX_ROGUE
#ifdef EX_MAGE
					case JOB_EX_MAGE:
						npcIndex=478;
						break;    // 아누비스 주술사
#endif // EX_MAGE
					}

					gserver->m_MonsterMercenary.SetMercenaryData( item, 3, npcIndex ); // B의 아이템으로 npc 세팅.
				}
#endif

				if(item->IsAccessary() && item->m_nOption < 1)
				{
					OptionSettingItem(ch, item);
				}

				if( item->IsAPet() )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperAttackPetMsg( rmsg, MSG_HELPER_APET_CREATE_REQ, ch->m_index );
					RefMsg(rmsg) << item->m_itemProto->getItemNum0();
					SEND_Q( rmsg, gserver->m_helper );
					break;
				}

				if (ch->m_inventory.addItem(item) == false)
				{
					// 인젠토리 꽉차서 못 받을 때
					// Quest Error Log
					GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;

					// 펫 아이템일 경우 꽉 차면 드롭시키지 않고 피리를 다시 집어 넣고 컨티뉴
					if( item->IsPet() )
					{
						int egg_index = 0;

						switch( item->m_itemProto->getItemIndex() )
						{
						case PET_HORSE_ITEM_INDEX:
							egg_index =	PET_HORSE_EGG_INDEX;
							break;
						case PET_DRAGON_ITEM_INDEX:
							egg_index =	PET_DRAGON_EGG_INDEX;
							break;
						case PET_BLUE_HORSE_ITEM_INDEX:
							egg_index =	PET_BLUE_HORSE_EGG_INDEX;
							break;
						case PET_PINK_DRAGON_ITEM_INDEX:
							egg_index =	PET_PINK_DRAGON_EGG_INDEX;
							break;
						case PET_UNKOWN_HORSE_ITEM_INDEX:
							egg_index =	PET_UNKOWN_HORSE_EGG_INDEX;
							break;
						case PET_UNKOWN_DRAGON_ITEM_INDEX:
							egg_index =	PET_UNKOWN_DRAGON_EGG_INDEX;
							break;
						default :
							continue;
						}

						// Item 수량 변경
						ch->m_inventory.deleteItemByDBIndex(item->getDBIndex(), pQuestProto->m_prizeData[i]);

						ch->GiveItem(egg_index, 0, 0, 1, true);
						CNetMsg::SP rmsg(new CNetMsg);
						QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
						SEND_Q( rmsg, ch->m_desc);

						continue;
					}

					// 인벤토리 가득참
					item = ch->m_pArea->DropItem(item, ch);
					if (!item)
						continue;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						item->m_preferenceIndex = ch->m_index;
						ItemDropMsg(rmsg, ch, item);
						ch->m_pArea->SendToCell(rmsg, ch, true);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRIZE_FULL);
						SEND_Q(rmsg, ch->m_desc);
					}
				}

				// Item LOG
				GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
						<< itemlog(item)
						<< end;

				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_ITEM << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;

				// 펫 퀘스트 일 경우 헬퍼에 생성 요청 : 할일
				if ( pQuest->IsPetQuest() )
				{
					if( gserver->isRunHelper() )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						switch( item->m_itemProto->getItemIndex() )
						{
						case PET_HORSE_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_HORSE | PET_GRADE_CHILD) );
							break;
						case PET_DRAGON_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_DRAGON | PET_GRADE_CHILD) );
							break;
						case PET_BLUE_HORSE_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_BLUE_HORSE | PET_GRADE_CHILD) );
							break;
						case PET_PINK_DRAGON_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_PINK_DRAGON | PET_GRADE_CHILD) );
							break;
						case PET_UNKOWN_HORSE_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_UNKOWN_HORSE | PET_GRADE_CHILD) );
							break;
						case PET_UNKOWN_DRAGON_ITEM_INDEX:
							HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_UNKOWN_DRAGON | PET_GRADE_CHILD) );
							break;
						default :
							continue;
						}

						SEND_Q(rmsg, gserver->m_helper);
					}
					else
					{
						switch( item->m_itemProto->getItemIndex() )
						{
						case PET_HORSE_ITEM_INDEX:
							ch->GiveItem(PET_HORSE_EGG_INDEX, 0,0, 1, true);
							break;
						case PET_DRAGON_ITEM_INDEX:
							ch->GiveItem(PET_DRAGON_EGG_INDEX, 0,0, 1, true);
							break;
						case PET_BLUE_HORSE_ITEM_INDEX:
							ch->GiveItem(PET_BLUE_HORSE_EGG_INDEX, 0,0, 1, true);
							break;
						case PET_PINK_DRAGON_ITEM_INDEX:
							ch->GiveItem(PET_PINK_DRAGON_EGG_INDEX, 0,0, 1, true);
							break;
						case PET_UNKOWN_HORSE_ITEM_INDEX:
							ch->GiveItem(PET_UNKOWN_HORSE_EGG_INDEX, 0,0, 1, true);
							break;
						case PET_UNKOWN_DRAGON_ITEM_INDEX:
							ch->GiveItem(PET_UNKOWN_DRAGON_EGG_INDEX, 0,0, 1, true);
							break;
						default :
							continue;
						}

						if( item )
						{
							ch->m_inventory.deleteItemByDBIndex(item->getDBIndex(), 1);
						}

						CNetMsg::SP rmsg(new CNetMsg);
						FailMsg(rmsg, MSG_FAIL_SYSTEM_ERROR);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
			}

			break;

		case QPRIZE_MONEY:
			{
				ch->m_inventory.increaseMoney(pQuestProto->m_prizeData[i]);

				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_MONEY << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
			}
			break;

		case QPRIZE_EXP:
			{
				// 보상 경험치 주기
				LONGLONG exp = ch->m_exp;
				LONGLONG prize_exp = pQuestProto->m_prizeData[i];

				if(ch->m_avPassiveAddition.quest_exp > 0)
				{
					prize_exp += ch->m_avPassiveAddition.quest_exp;
				}
				if(ch->m_avPassiveRate.quest_exp > 0)
				{
					prize_exp = prize_exp * (ch->m_avPassiveRate.quest_exp - 1000) / SKILL_RATE_UNIT;
				}
#ifdef CASHITEM_EXPSP_LIMIT_BUG
				ch->AddExpSP(prize_exp, 0, true, true);
#else
				ch->AddExpSP(pQuestProto->m_prizeData[i], 0, false, true);
#endif // CASHITEM_EXPSP_LIMIT_BUG
				exp = ch->m_exp - exp;
				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_EXP << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< "ORIGINAL EXP(" << pQuestProto->m_prizeData[i] << ")" << delim
						<< "TAKE EXP(" << exp << ")"
						<< end;
			}
			break;

		case QPRIZE_SP:
			{
				int sp = ch->m_skillPoint;
				// 보상 SP 주기
#ifdef CASHITEM_EXPSP_LIMIT_BUG
				ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, true, true);
#else
				ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, false, true);
#endif // CASHITEM_EXPSP_LIMIT_BUG
				sp = ch->m_skillPoint - sp;
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_SP << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< "ORIGINAL SP(" << pQuestProto->m_prizeData[i] << ")" << delim
						<< "TAKE SP(" << sp << ")"
						<< end;
			}
			break;

		case QPRIZE_SKILL:
			break;

		case QPRIZE_SSKILL:
			{
				// sskill index : pQuestProto->m_prizeIndex[i]
				// sskill level : pQuestProto->m_prizeData[i]
				CSSkillProto* proto = gserver->m_sSkillProtoList.FindProto(pQuestProto->m_prizeIndex[i]);
				if (!proto)
					continue;

				CSSkill* sskill = ch->m_sSkillList.Find(proto->m_index);

				sskill = new CSSkill(proto, pQuestProto->m_prizeData[i]);
				if (!sskill)
					continue;

				bool bAdd = true;		// 추가하는가?

				CSSkillNode* node;
				CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
				while ((node = nodeNext))
				{
					nodeNext = nodeNext->m_next;

					if ( (sskill->m_proto->m_index == node->m_sskill->m_proto->m_index) )
					{
						if (sskill->m_level > node->m_sskill->m_level)	// 습득 레벨 보다 높다.
							node->m_sskill->m_level = sskill->m_level;
						bAdd = false;
					}
				}

				if (bAdd)
				{
					if (!ch->m_sSkillList.Add(sskill))
						continue;

					// 스킬 배움 메시지 전송
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeSSkillLearn(rmsg, sskill);
					SEND_Q(rmsg, ch->m_desc);

					GAMELOG << init("QUEST PRIZE", ch)
							<< pQuestProto->m_index << delim
							<< QPRIZE_SSKILL << delim
							<< pQuestProto->m_prizeIndex[i] << delim
							<< pQuestProto->m_prizeData[i]
							<< end;
				}
				else
				{
					if (sskill)
						delete sskill;
				}
			}
		case QPRIZE_RVRPOINT:
			{
				GAMELOG << init("QUEST PRIZE", ch)
					<< pQuestProto->m_index << delim
					<< QPRIZE_RVRPOINT << delim
					<< ch->m_syndicateManager.getSyndicatePoint(ch->getSyndicateType()) << delim
					<< pQuestProto->m_prizeData[i]
				<< end;

				ch->m_syndicateManager.increaseSyndicatePoint(pQuestProto->m_prizeData[i]);
			}
			break;
		default:
			continue;
		}
	}
#ifdef GER_LOG
	for( int rewardItemCount = 0 ; rewardItemCount < 5 ; rewardItemCount++ )
	{
		GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
				   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
				   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
				   << LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
				   << LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
				   << LOG_VAL("end-type", pQuest->GetQuestProto()->m_prizeNPC ) << blank
				   << LOG_VAL("npc-id", pQuest->GetPrizeNPCIndex() ) << blank
				   << LOG_VAL("prize-type", pQuest->GetQuestProto()->m_prizeType[rewardItemCount]) << blank
				   << LOG_VAL("item-id", pQuest->GetQuestProto()->m_prizeIndex[rewardItemCount] ) << blank
				   << LOG_VAL("amount", pQuest->GetQuestProto()->m_prizeData[rewardItemCount] ) << blank
				   << endGer;
	}
#endif // GER_LOG
	// 추가 보상 아이템
	if(optItemIndex != -1)
	{
		//추가 아이템은 하나뿐이다.
		bool bFind = false;
		bool bFoundSameType = false;
		CItemProto* pItemProtoNeed = gserver->m_itemProtoList.FindIndex(optItemIndex);

		for (i=0; i < QUEST_MAX_OPTPRIZE; i++)
		{
			if (pQuestProto->m_optPrizeType[i] == -1)
			{
				continue;
			}
			else if(pQuestProto->m_optPrizeIndex[i] == optItemIndex)
			{
				bFind = true;
				bFoundSameType = true;
				break;
			}
			else if (pItemProtoNeed)
			{
				CItemProto* pItemProtoFind = gserver->m_itemProtoList.FindIndex(pQuestProto->m_optPrizeIndex[i]);

				if (   pItemProtoFind
						&& pItemProtoFind->getItemTypeIdx() == pItemProtoNeed->getItemTypeIdx()
						&& pItemProtoFind->getItemSubTypeIdx() == pItemProtoNeed->getItemSubTypeIdx()
						&& pItemProtoFind->GetItemProtoLevel() == pItemProtoNeed->GetItemProtoLevel())
				{
					bFoundSameType = true;
					//break;
				}
			}
		}
		if(!bFind)
		{
			CItemProto* proto = gserver->m_itemProtoList.FindIndex(optItemIndex);

			if(bFoundSameType && proto && proto->getItemTypeIdx() == ITYPE_WEAR)
			{
				if(plus < 0 || plus > 4) // +4이상의 아이템을 보상으로 주진 않는다. //  [2/28/2007 KwonYongDae]
				{
					plus = 0;
				}

				CItem* item = gserver->m_itemProtoList.CreateItem(optItemIndex, -1, plus, 0, 1);	// 방어구라서 count는 무조건 1로 함

				if (!item)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRZIE_ITEM);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				if(item->IsAccessary() && item->m_nOption < 1)
				{
					OptionSettingItem(ch, item);
				}

				if (ch->m_inventory.addItem(item) == false)
				{
					// 인젠토리 꽉차서 못 받을 때
					// Quest Error Log
					GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;

					// 인벤토리 가득참
					item = ch->m_pArea->DropItem(item, ch);
					if (!item)
						return;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						item->m_preferenceIndex = ch->m_index;
						ItemDropMsg(rmsg, ch, item);
						ch->m_pArea->SendToCell(rmsg, ch, true);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRIZE_FULL);
						SEND_Q(rmsg, ch->m_desc);
					}
				}
			}
			else
			{
				LOG_ERROR("Invalid item index[%d]", optItemIndex);
			}
		}
		else
		{
			if (pQuestProto->m_optPrizeType[i] == -1)
				return;

			switch (pQuestProto->m_optPrizeType[i])
			{
			case QPRIZE_ITEM:
				// 추가 보상 아이템 주기
				{
					if(plus < 0 || plus > 4) // +4 이상의 아이템을 보상으로 주진 않는다.
					{
						plus = 0;
					}

					CItem* item = gserver->m_itemProtoList.CreateItem(optItemIndex, -1, plus, 0, pQuestProto->m_optPrizeData[i]);

					if (!item)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRZIE_ITEM);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					if(item->IsAccessary() && item->m_nOption < 1)
					{
						OptionSettingItem(ch, item);
					}

					if (ch->m_inventory.addItem(item))
					{
						// Item LOG
						GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
								<< itemlog(item)
								<< end;

						// Quest Prize Log
						GAMELOG << init("QUEST PRIZE", ch)
								<< pQuestProto->m_index << delim
								<< QPRIZE_ITEM << delim
								<< pQuestProto->m_optPrizeIndex[i] << delim
								<< pQuestProto->m_optPrizeData[i]
								<< end;
					}
					else
					{
						// 인젠토리 꽉차서 못 받을 때
						// Quest Error Log
						GAMELOG << init("QUEST ERROR", ch)
								<< pQuestProto->m_index << delim
								<< (int)MSG_QUEST_ERR_PRIZE_FULL
								<< end;

						// 인벤토리 가득참
						item = ch->m_pArea->DropItem(item, ch);
						if (!item)
							return;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							item->m_preferenceIndex = ch->m_index;
							ItemDropMsg(rmsg, ch, item);
							ch->m_pArea->SendToCell(rmsg, ch, true);
						}

						{
							CNetMsg::SP rmsg(new CNetMsg);
							QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRIZE_FULL);
							SEND_Q(rmsg, ch->m_desc);
						}
					}
				}

				break;
			case QPRIZE_MONEY:
				{
					ch->m_inventory.increaseMoney(pQuestProto->m_prizeData[i]);

					// Item LOG
					GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
							<< delim
							<< ch->m_inventory.getMoney()
							<< end;
					//}

					// Quest Prize Log
					GAMELOG << init("QUEST PRIZE", ch)
							<< pQuestProto->m_index << delim
							<< QPRIZE_MONEY << delim
							<< pQuestProto->m_prizeIndex[i] << delim
							<< pQuestProto->m_prizeData[i]
							<< end;
				}
				break;

			case QPRIZE_EXP:
				// 보상 경험치 주기
#ifdef CASHITEM_EXPSP_LIMIT_BUG
				ch->AddExpSP((LONGLONG)pQuestProto->m_prizeData[i], 0, true, true);
#else
				ch->AddExpSP((LONGLONG)pQuestProto->m_prizeData[i], 0, false, true);
#endif // CASHITEM_EXPSP_LIMIT_BUG

				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_EXP << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
				break;

			case QPRIZE_SP:
				// 보상 SP 주기
#ifdef CASHITEM_EXPSP_LIMIT_BUG
				ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, true, true);
#else
				ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, false, true);
#endif // CASHITEM_EXPSP_LIMIT_BUG

				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_SP << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
				break;

			case QPRIZE_SKILL:
				break;

			default: //추가보상은 아이템만 가능하다.
				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRIZE_FULL);
					SEND_Q(rmsg, ch->m_desc);
				}
				//return;
			}
		}//find의 else
	}

	{
		// Prize Msg Echo
		CNetMsg::SP rmsg(new CNetMsg);
		QuestPrizeMsg(rmsg, pQuest);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 1회용 퀘스트 저장
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
	}
	else if(pQuestProto->m_type[1] == QTYPE_REPEAT_DAY)
	{
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);

		// 삭제할 시간 저장 자정을 기준으로 무조건 갱신한다
		tm tmNext = NOW();
		tmNext.tm_mday = tmNext.tm_mday + 1;
		tmNext.tm_hour = 0;
		tmNext.tm_min = 0;
		tmNext.tm_sec = 0;
		tmNext.tm_isdst = -1;

		pQuest->SetCompleteTime(mktime(&tmNext));
	}
	// 캐릭터 리스트에서 삭제
	else
		ch->m_questList.DelQuest(ch, pQuest);

	bool bTicket = true;
	// 싱글던전 퀘스트이면 입장권 nComplete 증가 하지 않는다
	if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
		bTicket = false;

	if (bTicket)
	{
		// 온전하게 보상 받았다면
		ch->m_questList.IncreaseQuestComepleteCount();

		// 5회 이상이고 싱글던전1,2의 레벨 조건이면
		if (ch->m_questList.GetQuestComepleteCount() >= S2_TICKET_QUEST_DONE && ch->m_level >= S2_TICKET_LEVEL_START && ch->m_level <= S2_TICKET_LEVEL_END)
		{
			ch->m_questList.ResetQuestComepleteCount();

			CItem* item = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_sTicketItem2->getItemIndex(), -1, 0 ,0, 1);

			if (!item)
				return;

			if (ch->m_inventory.addItem(item))
			{
				GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
						<< itemlog(item)
						<< end;
			}
			else
			{
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				CNetMsg::SP rmsg(new CNetMsg);
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, item);
				ch->m_pArea->SendToCell(rmsg, ch, true);
				return ;
			}
		}
	}

	CQuestProto* questproto = gserver->m_questProtoList.FindProto(questIndex);
	vec_quest_affinityList_t::iterator it = questproto->m_affinityList.begin();
	vec_quest_affinityList_t::iterator endit = questproto->m_affinityList.end();
	int point = 0;
	for(; it != endit; ++it)
	{
		CAffinityProto* affinityproto = *(it);

		if(affinityproto == NULL)
			continue;

		CAffinity* affinity = ch->m_affinityList.FindAffinity(affinityproto->m_index);
		if(affinity)
		{
			point = affinityproto->GetAffinityPointOfQuest(questproto->m_index);
			int bonus = 0;
			if(ch->m_avPassiveAddition.affinity_quest > 0)
			{
				bonus += ch->m_avPassiveAddition.affinity_quest;
			}
			if(ch->m_avPassiveRate.affinity_quest > 0)
			{
				bonus = point * (ch->m_avPassiveRate.affinity_quest - 1000) / SKILL_RATE_UNIT;
			}
			affinity->AddPoint(point, ch, bonus);
		}
	}
}

void do_QuestComplete(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	int questIndex;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
		>> questIndex;

	CQuest* pQuest = ch->m_questList.FindQuest(questIndex);

	if (!pQuest || pQuest->GetQuestState() != QUEST_STATE_RUN)
		return ;

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();

	if (!pQuestProto)
		return ;

	int i;

	switch (pQuestProto->m_type[0])
	{
	// 구출 퀘스트 완료 Validation 검사!!
	case QTYPE_KIND_SAVE:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				// 아예 없으면
				if (pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						item_search_t vec;
						int search_count = ch->m_inventory.searchItemByCondition(pQuestProto->m_conditionIndex[i], 0, 0, vec);
						if (search_count == 0 || search_count < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
									<< pQuestProto->m_index
									<< end;

							CNetMsg::SP rmsg(new CNetMsg);
							QuestFailMsg(rmsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;
				case QCONDITION_NPC:
					{
						pQuest->IncreaseQuestValue(i);
					}
					break;
				default:
					break;
				}
			}
		}
		break;

	case QTYPE_KIND_TUTORIAL:
		{
			CItem* item = NULL;

			for (i=0; i < gserver->m_itemProtoList.m_nCount; i++)
			{
				if (gserver->m_itemProtoList.m_protoItems[i].getItemTypeIdx() != ITYPE_WEAR)
					continue;
				if (gserver->m_itemProtoList.m_protoItems[i].getItemSubTypeIdx() != IWEAR_ARMOR)
					continue;
				if (gserver->m_itemProtoList.m_protoItems[i].GetItemProtoLevel() != 6)
					continue;
				if (!(gserver->m_itemProtoList.m_protoItems[i].getItemJobFlag() & (1 << ch->m_job)))
					continue;

				item = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_protoItems[i].getItemIndex(), -1, 0, 0, 1);
				break;
			}

			if (!item)
				return;

			if (ch->m_inventory.addItem(item) == false)
			{
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				// Drop Msg 보내기
				CNetMsg::SP rmsg(new CNetMsg);
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, item);
				ch->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}

			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
					<< itemlog(item)
					<< end;

			// Complete Msg 최초전송인경우에만 전송

			if (!pQuest->IsCompleted() || pQuest->GetQuestIndex() == 45)
			{
				//if(ch->m_questList.m_bComplete2[idx])
				{
#ifdef GER_LOG
					for( int i = 0;  i < 5 /*pQuest->GetQuestProto()->m_optPrizeIndex*/ ; i ++ )
					{
						if( pQuest->GetQuestProto()->m_prizeIndex[i] == 0 )
							continue;
						GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
								   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
								   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
								   << LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
								   << LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
								   << LOG_VAL("end-type", pQuest->GetQuestProto()->m_prizeNPC ) << blank
								   << LOG_VAL("npc-id", pQuest->GetPrizeNPCIndex() ) << blank
								   << LOG_VAL("prize-type", pQuest->GetQuestProto()->m_prizeType[i]) << blank
								   << LOG_VAL("item-id", pQuest->GetQuestProto()->m_prizeIndex[i] ) << blank
								   << LOG_VAL("amount", pQuest->GetQuestProto()->m_prizeData[i] ) << blank
								   << endGer;
					}
#endif // GER_LOG
					// Quest Complete Log
					GAMELOG << init("QUEST COMPLETE (do_QuestComplete)", ch)
							<< pQuestProto->m_index
							<< end;

					// 완료 메시지
					CNetMsg::SP rmsg(new CNetMsg);
					QuestCompleteMsg(rmsg, pQuest);
					SEND_Q(rmsg, ch->m_desc);
				}
				pQuest->SetComplete1(true);
			}

//#ifdef GER_LOG
//			GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
//				<< LOG_VAL("doFuncQuest.cpp",":3327") << blank
//				<< LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
//				<< LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
//				<< LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
//				<< LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
//				<< LOG_VAL("npc-id", pQuest->GetPrizeNPCIndex() ) << blank
//				<< endGer;
//#endif // GER_LOG
			// Quest Prize Log
			GAMELOG << init("QUEST PRIZE",  ch->m_nick, ch->m_nick)
					<< pQuestProto->m_index << delim
					<< QPRIZE_MONEY << delim
					<< pQuestProto->m_prizeIndex[0] << delim
					<< pQuestProto->m_prizeData[0]
					<< end;

			{
				// Prize Msg Echo
				CNetMsg::SP rmsg(new CNetMsg);
				QuestPrizeMsg(rmsg, pQuest);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 1회용 퀘스트 저장
			if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
			{
				ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
			}
			else
			{
				ch->m_questList.DelQuest(ch, pQuest);
			}

			ch->m_newtutoComplete = 0;
			return;
		}
		break;

	default:
		return;
	}

	// Complete Msg 최초전송인경우에만 전송
	if (!pQuest->IsCompleted())
	{
		if(pQuest->GetComplete2())
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (do_QuestComplete2)", ch)
					<< pQuestProto->m_index
					<< end;
#ifdef GER_LOG
			for( int rewardItemCount = 0 ; rewardItemCount < 5 ; rewardItemCount++ )
			{
				GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
						   << LOG_VAL("doFuncQuest.cpp",":3444") << blank
						   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
						   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
						   << LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
						   << LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
						   << LOG_VAL("end-type", pQuest->GetQuestProto()->m_prizeNPC ) << blank
						   << LOG_VAL("npc-id", pQuest->GetPrizeNPCIndex() ) << blank
						   << LOG_VAL("item-id", pQuest->GetQuestProto()->m_prizeIndex[rewardItemCount] ) << blank
						   << LOG_VAL("amount", pQuest->GetQuestProto()->m_prizeData[rewardItemCount] ) << blank
						   << endGer;
			}
#endif //GER_LOG
			// 완료 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCompleteMsg(rmsg, pQuest);
			SEND_Q(rmsg, ch->m_desc);
		}
		pQuest->SetComplete1(true);
	}
}

CItem* OptionSettingItem(CPC* ch, CItem* item)
{
	if (item->IsRareItem())
		return item;

	if (item->IsOriginItem())
		return item;

	if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_CASH ))
		return item;

	// 악세사리 일때 Option Setting

	// 순서대로 5,4,3,1개 붙을 확률
	const int factor[] = {10, 40, 80, 100};
	const int limit[] = {500, 2000, 4000, 5000};

	int prob[MAX_ACCESSORY_OPTION-1];
	int i, j;

	for (i=0; i < MAX_ACCESSORY_OPTION - 1; i++)
	{
		if ((prob[i] = ch->m_level * factor[i]) > limit[0])
			prob[i] = limit[i];
	}

	// 옵션 수 결정
	int result = GetRandom(1, 10000);
	int num = 2;	// default 2개
	int* type;

	if (result <= prob[0])
		num = 5;
	else if (result <= prob[1])
		num = 4;
	else if (result <= prob[2])
		num = 3;
	else if (result <= prob[3])
		num = 1;
	else
		num = 2;

	type = new int[num];

	LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

	// 옵션 타입 결정
	i = 0;
	while (i < num)
	{
		// type[i] 결정
		type[i] = GetRandom(0, maxnumoption - 1);

		// type[i]가 중복인 검사
		// 중복이면 type[i]를 1씩 증가하면서 중복 검사
		for (j = 0; j < maxnumoption; j++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
				break;
		}

		// 더이상 옵션을 붙일 수 없음
		if (j == maxnumoption)
			num = i;
		else
			type[i] = (type[i] + j) % maxnumoption;

		bitfield |= ((LONGLONG)1 << type[i]);

		// 옵션번호로 붙을 수 있는 옵션인지 검사
		COptionProto* proto = gserver->m_optionProtoList.FindProto(type[i]);
		if (!proto)
			continue;

		// 악세사리중에서 붙을 수 없으면 다시
		if (!((1 << item->m_itemProto->getItemSubTypeIdx()) & proto->m_accessoryType))
			continue ;

		i++;
	}

	// 옵션 부여
	for (i=0; i < num; i++)
	{
		COptionProto* proto = gserver->m_optionProtoList.FindProto(type[i]);

		if (!proto)
			continue;

		// Level, Value 셋팅
		item->m_option[item->m_nOption].MakeOptionValue(proto, ch->m_level, num);
		++item->m_nOption;
	}

	delete [] type;

	return item;
}

void do_QuestCollect(CPC* ch, CNetMsg::SP& msg)
{
	int			nNPCIndex;
	int			nItemIndex = -1;

	RefMsg(msg) >> nNPCIndex;

	// 이전 수집 시간 검사 : 2초(PULSE_PRODUCE_DELAY)
	if (gserver->m_pulse - ch->m_nLastCollectRequest < PULSE_PRODUCE_DELAY - PULSE_HACK_ATTACK_THRESHOLD)
		return ;

	// NPC 찾기
	CNPC* pNPC = (CNPC*)ch->m_pArea->FindCharInCell(ch, nNPCIndex, MSG_CHAR_NPC);
	if (!pNPC)
		return ;

	// NPC와의 거리 검사 : 5미터
	if (GetDistance(ch, pNPC) >= pNPC->m_proto->m_attackArea + 0.5)
		return ;

	// 이전 NPC와 동일 여부 검사
	if (ch->m_nCollectRequestNPCIndex != nNPCIndex)
		ch->m_nCollectRequestCount = 0;

	if( pNPC->m_proto->m_family > 0 )
		FindFamilyInNear( ch, pNPC );

	// 변수값 설정
	ch->m_nLastCollectRequest = gserver->m_pulse;
	ch->m_nCollectRequestNPCIndex = nNPCIndex;
	ch->m_nCollectRequestCount++;

	// 아이템 습득 여부 결정 : 4회
	if (ch->m_nCollectRequestCount >= 4)
	{
		// 변수 리셋
		ch->m_nLastCollectRequest = 0;

		// 아이템 결정
		int nMaxRandom = MAX_NPC_PRODUCT - 1;
		while (nMaxRandom >= 0)
		{
			int nProb = GetRandom(0, nMaxRandom);
			if (pNPC->m_proto->m_product[nProb] <= 0)
				nMaxRandom--;
			else
			{
				nItemIndex = pNPC->m_proto->m_product[nProb];
				break ;
			}
		}

		if (nItemIndex > 0)
		{
			// 아이템 생성
			CItem* pItem = gserver->m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, 1);
			if (pItem)
			{
				if (ch->m_inventory.addItem(pItem) == false)
				{
					delete pItem;
					pItem = NULL;
					nItemIndex = 0;
					GAMELOG << init("QUEST COLLECT FAIL" , ch)
							<< "FULL INVEN"
							<< end;
				}
			}
			else
			{
				nItemIndex = 0;
			}
		}
		else
		{
			nItemIndex = 0;
		}
		{
			// 결과 알림
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCollectMsg(rmsg, nNPCIndex, ch->m_index, nItemIndex);
			pNPC->m_pArea->SendToCell(rmsg, pNPC, true);
		}

		// NPC 사망
		DelAttackList(pNPC);
		ch->m_pArea->CharFromCell(pNPC, true);
		ch->m_pArea->DelNPC(pNPC);
		pNPC = NULL;
	}
	else
	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		QuestCollectMsg(rmsg, nNPCIndex, ch->m_index, nItemIndex);
		pNPC->m_pArea->SendToCell(rmsg, pNPC, true);
	}
}

void do_QuestRestoreAbandon(CPC* ch, CNetMsg::SP& msg)
{
	LONGLONG clientNas = 0;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> clientNas;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("Quest RestoreAbandon FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_RESETSTAT) )
	{
		GAMELOG << init("Quest RestoreAbandon FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> clientNas;
#endif

	// 복구에 필요한 나스 계산
	LONGLONG necessaryNas = (ch->m_level) * (ch->m_level) * 1000;

	// 나스 계산이 서버와 틀리다.
	if (clientNas != necessaryNas)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_DONT_HAVE_NAS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 퀘스트 복구 검사
	int nRestoreCount = 0;
	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);

		// 레벨이 맞는 퀘스트만 복구시킨다.
		if(pQuest && pQuest->GetQuestProto() && pQuest->GetQuestProto()->m_needMinLevel <= ch->m_level && pQuest->GetQuestProto()->m_needMaxLevel + 4 >= ch->m_level)
			nRestoreCount++;
	}

	// 현재 레벨에 맞는 복구할 포기 퀘스트가 없다.
	if(nRestoreCount == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_NOT_EXIST_ABANDON_QUEST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 돈검사
	if ((ch->m_inventory.getMoney() >= necessaryNas))
	{
		ch->m_inventory.decreaseMoney(necessaryNas);
	}
	else
	{
		// 보유 나스가 부족
		CNetMsg::SP rmsg(new CNetMsg);
		QuestErrorMsg(rmsg, MSG_QUEST_ERR_DONT_HAVE_NAS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 레벨 시작형 퀘스트가 존재하는지 확인
	int index = 0;
	pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);

		if(pQuest && pQuest->GetQuestProto())
		{
			// 레벨 시작형 퀘스트 인지 검사
			if (pQuest->GetQuestProto()->m_startType != QSTART_LEVEL)
				continue;

			// 필요 직업 조건
			if (pQuest->GetQuestProto()->m_needJob != -1 && pQuest->GetQuestProto()->m_needJob != ch->m_job)
				continue;

			// rvr 퀘스트일 때 필요조건
			if (pQuest->GetQuestProto()->m_needRvRType > 0)
			{
				if(	pQuest->GetQuestProto()->m_needRvRType != ch->getSyndicateType() ||
						pQuest->GetQuestProto()->m_needRvRGrade > ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType()) )
				{
					continue;
				}
			}

			// 하드코딩 : 퍼스널 던전 퀘스트 , 튜토리얼 퀘스트 제외
			if (pQuest->GetQuestProto()->m_index == 42 || pQuest->GetQuestProto()->m_index == 45)
				continue;

			// 레벨이 맞는 퀘스트인지 검사
			if(pQuest->GetQuestProto()->m_needMinLevel <= ch->m_level && pQuest->GetQuestProto()->m_needMaxLevel + 4 >= ch->m_level)
			{
				index++;
			}
		}
	}

	if (index > 0)
	{
		int* qIndex = new int [index];
		index = 0;
		memset(qIndex, -1, sizeof(int) * index);

		// 레벨 시작형 퀘스트는 따로 시작하게끔 메세지를 보내준다.
		pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);

			if(pQuest && pQuest->GetQuestProto())
			{
				// 레벨 시작형 퀘스트 인지 검사
				if (pQuest->GetQuestProto()->m_startType != QSTART_LEVEL)
					continue;

				// 필요 직업 조건
				if (pQuest->GetQuestProto()->m_needJob != -1 && pQuest->GetQuestProto()->m_needJob != ch->m_job)
					continue;

				// rvr 퀘스트일 때 필요조건
				if (pQuest->GetQuestProto()->m_needRvRType > 0)
				{
					if(	pQuest->GetQuestProto()->m_needRvRType != ch->getSyndicateType() ||
							pQuest->GetQuestProto()->m_needRvRGrade > ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType()) )
					{
						continue;
					}
				}

				// 하드코딩 : 퍼스널 던전 퀘스트 , 튜토리얼 퀘스트 제외
				if (pQuest->GetQuestProto()->m_index == 42 || pQuest->GetQuestProto()->m_index == 45)
					continue;

				// 레벨이 맞는 퀘스트인지 검사
				if(pQuest->GetQuestProto()->m_needMinLevel <= ch->m_level && pQuest->GetQuestProto()->m_needMaxLevel + 4 >= ch->m_level)
				{
					qIndex[index] = pQuest->GetQuestIndex();
					index++;
				}
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCreateListMsg(rmsg, index, qIndex);
			SEND_Q(rmsg, ch->m_desc);
		}

		delete [] qIndex;
	}

	// 퀘스트 복구 시작
	int* nRestoreIndex = new int [nRestoreCount];
	index = 0;
	memset(nRestoreIndex, -1, sizeof(int) * nRestoreCount);

	pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);

		// 레벨이 맞는 퀘스트만 지운다.
		if(pQuest && pQuest->GetQuestProto() && pQuest->GetQuestProto()->m_needMinLevel <= ch->m_level && pQuest->GetQuestProto()->m_needMaxLevel + 4 >= ch->m_level)
		{
			nRestoreIndex[index] = pQuest->GetQuestIndex();
			index++;
			ch->m_questList.DelQuest(ch, pQuest, QUEST_STATE_ABANDON);
		}
	}

	{
		// 퀘스트 복구 완료
		CNetMsg::SP rmsg(new CNetMsg);
		QuestRestoreAbandonMsg(rmsg, nRestoreCount, nRestoreIndex);
		SEND_Q(rmsg, ch->m_desc);
	}

	delete [] nRestoreIndex;
}

void do_QuestItemReq(CPC* ch, CNetMsg::SP& msg)
{
	// 이부분은 이미 완료한 퀘스트인데 아이템이 없어져서, 사용해서 없을때, 그런데 또 필요할때 퀘스트아이템을 지급해주는 부분입니다.
	// 보상부분에서 퀘스트 아이템만 지급을 해줍니다. 일반 아이템의 경우는 절대 지급해줄 수 없습니다.
	int nQuestIndex = 0;
	int nItemIndex[5] = {0,};
	LONGLONG nItemCount[5] = {0,};

	RefMsg(msg) >> nQuestIndex;

	if(nQuestIndex == 0)
		return ;

	/*	CQuest* pQuest = NULL;
		pQuest = ch->m_questList.FindQuest(nQuestIndex, QUEST_STATE_DONE);
		if(pQuest == NULL)
		{
			// 요청한 퀘스트가 리스트에 없음. 받을 수 없음.
			return ;
		}

		if(pQuest->GetQuestState() != QUEST_STATE_DONE)
		{
			//퀘스트가 있으나 현재 수행중 받을 수 없음. 받을 수 없음.
			return ;
		}*/

	if(!ch->m_questList.FindQuest(nQuestIndex, QUEST_STATE_DONE))
	{
		// 퀘스트 종료 못했음.
		return ;
	}

	CQuestProto* pQuestProto = NULL;
	pQuestProto = gserver->m_questProtoList.FindProto(nQuestIndex);

	if(!pQuestProto)
		return ;

	int i;
	int NoItem = 0;
	for(i = 0; i < QUEST_MAX_PRIZE; i++)
	{
		if(pQuestProto->m_prizeType[i] == QPRIZE_ITEM)
		{
			if (ch->m_inventory.getEmptyCount() < 1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);

				return ;
			}

			nItemIndex[i] = pQuestProto->m_prizeIndex[i];
			nItemCount[i] = pQuestProto->m_prizeData[i];

			CItem* pItem = gserver->m_itemProtoList.CreateItem(nItemIndex[i], WEARING_NONE, 0, 0, nItemCount[i]);

			if(pItem == NULL)
				continue;

			if (nItemIndex[i] < 1)
			{
				delete pItem;
				pItem = NULL;
				continue;
			}

			if(pItem->m_itemProto->getItemTypeIdx() != ITYPE_ETC && pItem->m_itemProto->getItemSubTypeIdx() != IETC_QUEST)
			{
				delete pItem;
				pItem = NULL;
				continue;
			}

			// 여기까지 왔으면..퀘스트 아이템까지 걸러낸거고..인벤에 지급해주면 되나?
			if (ch->m_inventory.addItem(pItem) == false)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);

				delete pItem;

				return;
			}
		}
		else
		{
			NoItem++;
		}
	}
	if(NoItem == QUEST_MAX_PRIZE)
	{
		// 받을 아이템 없음.
		return ;
	}
}
//
