// Affinity.cpp: implementation of the CAffinity class.
// svn commit test2
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "Affinity.h"
#include "CmdMsg.h"

CAffinityNPC::CAffinityNPC()
{
	m_reward = NULL;
	m_totalreward = 0;
}

CAffinityNPC::~CAffinityNPC()
{
	if(m_reward)
	{
		delete[] m_reward;
		m_reward = NULL;
	}

	m_totalreward = 0;
}

bool CAffinityNPC::LoadRewardItem()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_affinity_reward_item WHERE a_npcidx = %d ORDER BY a_allow_point", m_npcidx);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_totalreward = cmd.m_nrecords;
	m_reward = new REWARDITEM[cmd.m_nrecords];
	int k = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_allow_point",  m_reward[k].allowpoint);

		cmd.GetRec("a_itemidx", m_reward[k].itemidx);
		cmd.GetRec("a_flag", m_reward[k].itemflag);
		cmd.GetRec("a_count", m_reward[k].itemcount);
		cmd.GetRec("a_exp", m_reward[k].exp);
		cmd.GetRec("a_sp", m_reward[k].sp);
		cmd.GetRec("a_needpclevel", m_reward[k].needPCLevel);
		cmd.GetRec("a_needitemidx", m_reward[k].needItemIdx);
		cmd.GetRec("a_needitemcount", m_reward[k].needItemCount);

		k++;
	}
	return true;
}

REWARDITEM* CAffinityNPC::GetNextReward(int receivePoint)
{
	int i;
	for(i = 0; i < m_totalreward; ++i)
	{
		if(receivePoint < m_reward[i].allowpoint)
		{
			return &(m_reward[i]);
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAffinityProto::CAffinityProto()
{
	m_npcWork = NULL;
	m_itemWork = NULL;
	m_questWork = NULL;
	m_affinityNPC = NULL;

	m_npcWorkCount = 0;
	m_itemWorkCount = 0;
	m_questWorkCount = 0;

	m_needAffinIdx = 0;
	m_needAffinPoint = 0;
	m_needPCLevel = 0;
	m_needItemIdx = 0;
	m_needItemCount = 0;
}

CAffinityProto::~CAffinityProto()
{
	if(m_npcWork)
	{
		delete[] m_npcWork;
		m_npcWork = NULL;
	}

	if(m_itemWork)
	{
		delete[] m_itemWork;
		m_itemWork = NULL;
	}

	if(m_questWork)
	{
		delete[] m_questWork;
		m_questWork = NULL;
	}

	if(m_affinityNPC)
	{
		delete[] m_affinityNPC;
		m_affinityNPC = NULL;
	}

	m_npcWorkCount = 0;
	m_itemWorkCount = 0;
	m_questWorkCount = 0;
	m_affinityNPCCount = 0;

	m_needAffinIdx = 0;
	m_needAffinPoint = 0;
	m_needPCLevel = 0;
	m_needItemIdx = 0;
	m_needItemCount = 0;
}

int CAffinityProto::GetAffinityPointOfNPC(int npcidx)
{
	map_AFFINITYWORK_t::iterator it = map_npcWork_.find(npcidx);
	if (it != map_npcWork_.end())
	{
		return it->second->value;
	}

	return 0;
}

int CAffinityProto::GetAffinityPointOfQuest(int questidx)
{
	map_AFFINITYWORK_t::iterator it = map_questWork_.find(questidx);
	if (it != map_questWork_.end())
	{
		return (it->second->value);
	}

	return 0;
}

int CAffinityProto::GetAffintiyPointOfItem(int itemidx)
{
	map_AFFINITYWORK_t::iterator it = map_itemWork_.find(itemidx);
	if (it != map_itemWork_.end())
	{
		return (it->second->value);
	}

	return -1;
}

bool CAffinityProto::LoadAffinityWork()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_affinity_work WHERE a_affinity_idx = %d and a_work_type = %d ORDER BY a_type_idx ", m_index, 1);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_npcWorkCount = cmd.m_nrecords;
	this->m_npcWork = new AFFINITYWORK[cmd.m_nrecords];

	int i;
	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_type_idx", m_npcWork[i].dataidx);
		cmd.GetRec("a_value", m_npcWork[i].value);

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(m_npcWork[i].dataidx);
		if(proto)
		{
			proto->m_affinityList.push_back(this);
		}

		map_npcWork_.insert(map_AFFINITYWORK_t::value_type(m_npcWork[i].dataidx, &m_npcWork[i]));

		i++;
	}

	sql.Format("SELECT * FROM t_affinity_work WHERE a_affinity_idx = %d and a_work_type = %d ORDER BY a_type_idx ", m_index, 0);

	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_itemWorkCount = cmd.m_nrecords;
	this->m_itemWork = new AFFINITYWORK[cmd.m_nrecords];

	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_type_idx", m_itemWork[i].dataidx);
		cmd.GetRec("a_value", m_itemWork[i].value);

		CItemProto* proto = gserver->m_itemProtoList.FindIndex(m_itemWork[i].dataidx);
		if(proto)
		{
			proto->m_affinityList.push_back(this);
		}

		map_itemWork_.insert(map_AFFINITYWORK_t::value_type(m_itemWork[i].dataidx, &m_itemWork[i]));

		i++;
	}

	sql.Format("SELECT * FROM t_affinity_work WHERE a_affinity_idx = %d and a_work_type = %d ORDER BY a_type_idx ", m_index, 2);

	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_questWorkCount = cmd.m_nrecords;
	this->m_questWork = new AFFINITYWORK[cmd.m_nrecords];

	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_type_idx", m_questWork[i].dataidx);
		cmd.GetRec("a_value", m_questWork[i].value);

		CQuestProto* proto = gserver->m_questProtoList.FindProto(m_questWork[i].dataidx);
		if(proto)
		{
			proto->m_affinityList.push_back(this);
		}

		map_questWork_.insert(map_AFFINITYWORK_t::value_type(m_questWork[i].dataidx, &m_questWork[i]));

		i++;
	}

	return true;
}

bool CAffinityProto::LoadAffinityNPC()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_affinity_npc WHERE a_affinity_idx = %d AND a_enable = 1 ORDER BY a_use_point", m_index);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	this->m_affinityNPCCount = cmd.m_nrecords;
	m_affinityNPC = new CAffinityNPC[cmd.m_nrecords];

	int i;
	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_npcidx", m_affinityNPC[i].m_npcidx);
		cmd.GetRec("a_use_point", m_affinityNPC[i].m_shoppoint);
		cmd.GetRec("a_flag", m_affinityNPC[i].m_flag);

		if(!m_affinityNPC[i].LoadRewardItem())
			return false;

		map_affinityNPC_.insert(map_CAffinityNPC_t::value_type(m_affinityNPC[i].m_npcidx, &m_affinityNPC[i]));

		i++;
	}

	return true;
}

CAffinityNPC* CAffinityProto::FindAffinityNPC(int npcidx)
{
	map_CAffinityNPC_t::iterator it = map_affinityNPC_.find(npcidx);
	return (it != map_affinityNPC_.end()) ? it->second : NULL;
}

CAffinityProtoList::CAffinityProtoList()
{
	m_affinity = NULL;
	m_AffinityTotal = 0;
}

CAffinityProtoList::~CAffinityProtoList()
{
	if(m_affinity)
	{
		delete[] m_affinity;
		m_affinity = NULL;
	}
	m_AffinityTotal = 0;
}

bool CAffinityProtoList::LoadAffinity()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_affinity WHERE a_enable = 1 ORDER BY a_index");

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_affinity = new CAffinityProto[cmd.m_nrecords];
	m_AffinityTotal = cmd.m_nrecords;

	int i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_index", m_affinity[i].m_index);
		cmd.GetRec("a_name", m_affinity[i].m_name);
		cmd.GetRec("a_maxvalue", m_affinity[i].m_maxvalue);
		cmd.GetRec("a_nas", m_affinity[i].m_nas);
		cmd.GetRec("a_needitemidx", m_affinity[i].m_needItemIdx);
		cmd.GetRec("a_needitemcount", m_affinity[i].m_needItemCount);
		cmd.GetRec("a_needlevel", m_affinity[i].m_needPCLevel);
		cmd.GetRec("a_affinity_idx", m_affinity[i].m_needAffinIdx);
		cmd.GetRec("a_affinity_value", m_affinity[i].m_needAffinPoint);

		if(!m_affinity[i].LoadAffinityWork())
			return false;

		if(!m_affinity[i].LoadAffinityNPC())
			return false;

		if (m_affinity[i].m_maxvalue > 2100000000)
		{
			LOG_ERROR("Affinity Max Value IS INTEGER MAX OVER Affinity_Index[%d], Affinity_name[%s]", m_affinity[i].m_index, (const char *)m_affinity[i].m_name);
			return false;
		}

		map_.insert(map_t::value_type(m_affinity[i].m_index, &m_affinity[i]));

		i++;
	}

	return true;
}

CAffinityProto* CAffinityProtoList::FindProtoByAffinityIdx(int idx)
{
	map_t::iterator it = map_.find(idx);
	return (it != map_.end()) ? it->second : NULL;
}

CAffinityProto* CAffinityProtoList::FindProtoByNPCIdx(int idx)
{
	int i;
	for(i = 0 ; i < m_AffinityTotal; ++i)
	{
		if(m_affinity[i].FindAffinityNPC(idx))
			return &(m_affinity[i]);
	}

	return NULL;
}

CAffinity::CAffinity()
{
	m_proto = NULL;
	m_affinityidx = 0;
	m_point = 0;
}

CAffinity::~CAffinity()
{
	set_rewardstep_t::iterator it = m_rewardstep.begin();
	set_rewardstep_t::iterator endit = m_rewardstep.end();
	for (; it != endit; ++it)
	{
		delete *(it);
	}
	m_rewardstep.clear();
}

REWARDITEM* CAffinity::GetNextReward(int npcidx)
{
	// 보상받은 단계찾기
	REWARDSTEP* step = FindRewardInfo(npcidx);
	if(!step)
		return NULL;

	// 친화도 NPC 찾기
	CAffinityNPC* npc = m_proto->FindAffinityNPC(npcidx);
	if(!npc)
		return NULL;

	REWARDITEM* rewarditem;
	rewarditem = npc->GetNextReward(step->point);
	return rewarditem;
}

REWARDSTEP* CAffinity::FindRewardInfo(int npcidx)
{
	set_rewardstep_t::iterator it = m_rewardstep.begin();
	set_rewardstep_t::iterator endit = m_rewardstep.end();
	for (; it != endit; ++it)
	{
		REWARDSTEP* step = *(it);
		if (step->affinitynpc == npcidx)
		{
			return step;
		}
	}

	return NULL;
}

int CAffinity::AddPoint(int point, CPC* pc, int bonus)
{
	if(point <= 0)
		return 0;

	int  prevPoint = m_point;
	LONGLONG sumdata = m_point + point + bonus;

	bool bBoostered = false;
	if(pc->m_assist.m_avAddition.affinity > 0)
	{
		bBoostered = true;
		sumdata += pc->m_assist.m_avAddition.affinity;
	}
	if(pc->m_assist.m_avRate.affinity_rate > 0)
	{
		bBoostered = true;
		sumdata += point * pc->m_assist.m_avRate.affinity_rate / 100;
	}

	if (sumdata > m_proto->m_maxvalue)
	{
		sumdata = m_proto->m_maxvalue;
	}

	m_point = (int)sumdata;

	// 1. 포인트 업데이트 메시지 보내기
	// 2. 선물받을 것이 있으면 메시지 전송
	int i, k;
	char msgSendCount=0;
	int* checkRewardNPC = NULL;
	checkRewardNPC = new int[m_proto->m_affinityNPCCount];
	memset(checkRewardNPC, 0, sizeof(int) * m_proto->m_affinityNPCCount);
	for(i = 0; i < m_proto->m_affinityNPCCount; ++i)		// NPC Count
	{
		for(k = 0; k < m_proto->m_affinityNPC[i].m_totalreward; ++k)	// 보상 Count
		{
			if( prevPoint < m_proto->m_affinityNPC[i].m_reward[k].allowpoint  &&
					m_proto->m_affinityNPC[i].m_reward[k].allowpoint <= m_point)
			{
				if(m_proto->m_affinityNPC[i].m_reward[k].needPCLevel > pc->m_level)
					continue;

				if(m_proto->m_affinityNPC[i].m_reward[k].needItemIdx > 0)
				{
					item_search_t vec;
					int sc = pc->m_inventory.searchItemByCondition(m_proto->m_affinityNPC[i].m_reward[k].needItemIdx, 0, 0, vec);
					if (sc == 0 || sc < m_proto->m_affinityNPC[i].m_reward[k].needItemCount )
					{
						continue ;
					}
				}

				checkRewardNPC[(int)msgSendCount] = m_proto->m_affinityNPC[i].m_npcidx;
				msgSendCount++;
				break;
			}
		}
	}

	if(msgSendCount && checkRewardNPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityRewardNoticeMsg(rmsg, msgSendCount, checkRewardNPC);
		SEND_Q(rmsg, pc->m_desc);
	}

	if(m_point - prevPoint)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityUpdatePointMsg(rmsg, m_affinityidx, m_point, bBoostered, bonus);
		SEND_Q(rmsg, pc->m_desc);
	}

	if(checkRewardNPC)
	{
		delete[] checkRewardNPC;
		checkRewardNPC = NULL;
	}

	return (m_point - prevPoint);
}

void CAffinityList::SendRewardNotice(CPC* pc)
{
	std::set<int> listAffinReward;

	map_affinitylist_t::iterator it = m_list.begin();
	map_affinitylist_t::iterator endit = m_list.end();
	for( ; it != endit; ++it)
	{
		CAffinity* affinity = it->second;

		for(int i = 0; i < affinity->m_proto->m_affinityNPCCount; ++i)
		{
			// nextreward: 앞으로 받을 보상단계 (포인트 고려하지 않고)
			REWARDITEM* nextreward = affinity->GetNextReward(affinity->m_proto->m_affinityNPC[i].m_npcidx);
			if(!nextreward)
			{
				// 받을 보상아이템이 없다. 혹은 오류
				continue;
			}

			if(nextreward->needPCLevel > pc->m_level)
			{
				// 레벨이 부족하여 받지 못한다.
				continue;
			}

			if(nextreward->needItemIdx > 0)
			{
				item_search_t vec;
				int sc = pc->m_inventory.searchItemByCondition(nextreward->needItemIdx, 0, 0, vec);
				if (sc == 0 || sc < nextreward->needItemCount )
				{
					continue ;
				}
			}

			// 현재 친화도 점수가 앞으로 받을 보상단계를 만족하는지
			if(nextreward->allowpoint > affinity->m_point)
			{
				// 포인트가 부족하여 받지 못한다.
				continue;
			}

			listAffinReward.insert(affinity->m_proto->m_affinityNPC[i].m_npcidx);
		}
	}

	// 메시지 전송
	int rewardcount = listAffinReward.size();
	if(rewardcount > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_REWARD_NOTICE);
		RefMsg(rmsg) << rewardcount;

		std::set<int>::iterator setit = listAffinReward.begin();
		std::set<int>::iterator setendit = listAffinReward.end();
		for(; setit != setendit; ++setit)
		{
			RefMsg(rmsg) << (int)(*setit);
		}

		SEND_Q(rmsg, pc->m_desc);
	}
}

void CAffinity::UpdateRewardStep(int npcidx, int point)
{
	bool bUpdate = false;

	set_rewardstep_t::iterator it = m_rewardstep.begin();
	set_rewardstep_t::iterator endit = m_rewardstep.end();
	for (; it != endit; ++it)
	{
		REWARDSTEP* step = *(it);
		if(step->affinitynpc == npcidx)
		{
			step->point = point;
			bUpdate = true;
			break;
		}
	}

	if(!bUpdate)
	{
		GAMELOG << init("AFFINITY REWARD ERROR") << end;
	}
}

CAffinityList::CAffinityList()
{
}

CAffinityList::~CAffinityList()
{
	clear();
}

bool CAffinityList::AddAffinity(int affinityidx, int point)
{
	CAffinityProto* affinityProto;
	CAffinity* affinity = new CAffinity;
	affinity->m_affinityidx = affinityidx;
	affinity->m_point = point;

	affinityProto = gserver->m_affinityProtoList.FindProtoByAffinityIdx(affinityidx);

	if(!affinityProto)
	{
		delete affinity;
		affinity = NULL;

		return false;
	}

	affinity->m_proto = affinityProto;
	int i;
	for(i = 0; i < affinityProto->m_affinityNPCCount; ++i)
	{
		REWARDSTEP* rewardstep = new REWARDSTEP;
		rewardstep->affinitynpc = affinityProto->m_affinityNPC[i].m_npcidx;
		rewardstep->point = 0;

		affinity->m_rewardstep.insert(rewardstep);
	}

	m_list.insert(map_affinitylist_t::value_type(affinity->m_affinityidx, affinity));

	return true;
}

bool CAffinityList::UpdateRewardPoint(int npcidx, int point)
{
	map_affinitylist_t::iterator it = m_list.begin();
	map_affinitylist_t::iterator endit = m_list.end();
	for (; it != endit; ++it)
	{
		CAffinity* affinity = it->second;
		REWARDSTEP* step = affinity->FindRewardInfo(npcidx);
		if (step)
		{
			step->point = point;
			return true;
		}
	}

	return false;
}

CAffinity* CAffinityList::FindAffinity(int affinityidx)
{
	map_affinitylist_t::iterator it = m_list.find(affinityidx);
	return (it != m_list.end()) ? it->second : NULL;
}

void CAffinityList::clear()
{
	if (m_list.empty())
		return;

	map_affinitylist_t::iterator it = m_list.begin();
	map_affinitylist_t::iterator endit = m_list.end();
	for (; it != endit; ++it)
	{
		delete it->second;
	}
	m_list.clear();
}
