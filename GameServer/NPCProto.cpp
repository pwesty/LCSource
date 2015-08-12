#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "NPCProto.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

#ifdef PREMIUM_CHAR
#include "../ShareLib/packetType/ptype_premium_char.h"
#endif

///////////////////
// ANPCPtoro member

CNPCProto::CNPCProto()
	: m_name(MAX_CHAR_NAME_LENGTH + 1)
{
	m_index = -1;
	m_level = 0;
	m_exp = 0;
	m_price = 0;
	m_sight = 0;
	m_walkSpeed = 0.0f;
	m_runSpeed = 0.0f;
	m_size = 0.0f;
	m_hp = 0;
	m_mp = 0;
	m_attack = 0;
	m_magic = 0;
	m_defense = 0;
	m_resist = 0;
	m_flag = 0;
	m_recoverHP = 0;
	m_recoverMP = 0;
	m_family = -1;
	memset(m_item, -1, sizeof(int) * MAX_NPC_DROPITEM);
	memset(m_itemPercent, 0, sizeof(int) * MAX_NPC_DROPITEM);
	memset(m_product, -1, sizeof(int) * MAX_NPC_PRODUCT);

// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
	m_minplus = 0;			// 최소 플러스 수치
	m_maxplus = 0;			// 최대 플러스 수치
	m_probplus = 0;			// 플러스 붙을 확률
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기

	m_itemCount = 0;

	m_attackSpeed = 0;
	m_attackArea = 0;

	m_nSkill = 0;
	memset(m_skillIndex, 0, sizeof(int) * MAX_NPC_SKILL);
	memset(m_skillLevel, 0, sizeof(int) * MAX_NPC_SKILL);
	memset(m_skillProb, 0, sizeof(int) * MAX_NPC_SKILL);

	m_moveArea = 0;

	m_aitype = NPC_AI_NORMAL;
	m_aiflag = 0;
	m_aileader_flag = 0;
	m_ai_summonHp = 0;
	m_aileader_idx = 0;
	m_aileader_count = 0;

	m_nHit = 0;
	m_nDodge = 0;
	m_nMagicAvoid = 0;

	memset(m_jewel, -1, sizeof(int) * MAX_NPC_DROPJEWEL);
	memset(m_jewelPercent, 0, sizeof(int) * MAX_NPC_DROPJEWEL);
	m_jewelCount = 0;
	memset(m_jobdropitem, -1, sizeof(int) * JOBCOUNT);
	memset(m_jobdropitemprob, -1, sizeof(int) * JOBCOUNT);

	memset(m_dropallitem, -1, sizeof(int) * MAX_NPC_DROPITEM);
	memset(m_dropallitemprob, -1, sizeof(int) * MAX_NPC_DROPITEM);
	m_lifetime = 0;
}

CNPCProto::~CNPCProto()
{
}

#ifdef PREMIUM_CHAR
//XX - 프리미엄캐릭터 : 몬스터를 죽였을때 획득하는 EXP/SP 10% 증가
int CNPCProto::getExpForPremiumChar( int premium_type )
{
	int ori_exp = m_exp;
	int exp = 0;

	switch (premium_type)
	{
	case PREMIUM_CHAR_TYPE_FIRST:
		exp = (int)(ori_exp * 1.1f);
		break;

	default:
		exp = ori_exp;
		break;
	}

//	LOG_DEBUG("TEST (exp) : %d - %d", ori_exp, exp);

	return exp;
}

int CNPCProto::getSkillPointForPremiumChar( int premium_type )
{
	int ori_skillpoint = m_skillPoint;
	int skillpoint = 0;

	switch (premium_type)
	{
	case PREMIUM_CHAR_TYPE_FIRST:
		skillpoint = (int)(ori_skillpoint * 1.1f);
		break;

	default:
		skillpoint = ori_skillpoint;
		break;
	}

//	LOG_DEBUG("TEST (sp) : %d - %d", ori_skillpoint, skillpoint);

	return skillpoint;
}
#endif

///////////////////////
// CNPCProtoList member

CNPCProtoList::CNPCProtoList()
{
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;

	for (int i = 0; i < MAX_NPC_DROPITEM; ++i)
	{
		std::string tstr = boost::str(boost::format("a_item_%d") % i);
		a_item_str.push_back(tstr);

		tstr = boost::str(boost::format("a_item_percent_%d") % i);
		a_item_percent_str.push_back(tstr);

		tstr = boost::str(boost::format("a_jewel_%d") % i);
		a_jewel_str.push_back(tstr);

		tstr = boost::str(boost::format("a_jewel_percent_%d") % i);
		a_jewel_percent_str.push_back(tstr);
	}

	for (int i = 0; i < MAX_NPC_PRODUCT; ++i)
	{
		std::string tstr = boost::str(boost::format("a_product%d") % i);
		a_product_str.push_back(tstr);
	}

	for (int i = 0; i < MAX_NPC_SKILL; ++i)
	{
		std::string tstr = boost::str(boost::format("a_skill%d") % i);
		a_skill_str.push_back(tstr);
	}
}

CNPCProtoList::~CNPCProtoList()
{
	if (m_npcProtoList)
		delete [] m_npcProtoList;
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;
}

bool CNPCProtoList::Load()
{
	if (m_npcProtoList)
		delete [] m_npcProtoList;
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;

	CDBCmd dbNpc;
	dbNpc.Init(&gserver->m_dbdata);
	std::string sql = "SELECT * FROM t_npc WHERE a_enable=1 ORDER BY a_index";
	dbNpc.SetQuery(sql);
	dbNpc.Open();
	m_nCount = dbNpc.m_nrecords;
	m_npcProtoList = new CNPCProto[m_nCount];

	int i = 0;
	int j;
	float scale = 1;

	while (dbNpc.MoveNext())
	{
		int npcIndex = 0;
		int npcflag  = 0;
		int npcflag1 = 0;		//확장 플래그(추가)
		int npcStateFlag = 0;
		unsigned int npcattr;
		dbNpc.GetRec("a_index",			npcIndex);
		dbNpc.GetRec("a_flag",			npcflag);
		dbNpc.GetRec("a_flag1",			npcflag1);		//확장 플래그(추가)
		dbNpc.GetRec("a_state_flag",	npcStateFlag);

		if (npcIndex == 334 || npcIndex == 335 || npcIndex == 336)
			continue ;

		m_npcProtoList[i].m_bSydicateJoinGiveQuestNPC = false;
		if (npcIndex == 1540)
		{
			m_npcProtoList[i].m_bSydicateJoinGiveQuestNPC = true;
		}

		m_npcProtoList[i].m_index = npcIndex;
		m_npcProtoList[i].m_flag  = npcflag;
		m_npcProtoList[i].m_flag1 = npcflag1;		//확장 플래그(추가)
		m_npcProtoList[i].m_stateFlag = npcStateFlag;
		dbNpc.GetRec(NPC_NAME_FIELD,	m_npcProtoList[i].m_name, true);
		dbNpc.GetRec("a_level",			m_npcProtoList[i].m_level);
		dbNpc.GetRec("a_exp",			m_npcProtoList[i].m_exp);
		dbNpc.GetRec("a_prize",			m_npcProtoList[i].m_price);
		dbNpc.GetRec("a_sight",			m_npcProtoList[i].m_sight);
		dbNpc.GetRec("a_skill_point",	m_npcProtoList[i].m_skillPoint);
		dbNpc.GetRec("a_walk_speed",	m_npcProtoList[i].m_walkSpeed);
		dbNpc.GetRec("a_run_speed",		m_npcProtoList[i].m_runSpeed);
		dbNpc.GetRec("a_size",			m_npcProtoList[i].m_size);
		dbNpc.GetRec("a_recover_hp",	m_npcProtoList[i].m_recoverHP);
		dbNpc.GetRec("a_recover_mp",	m_npcProtoList[i].m_recoverMP);
		dbNpc.GetRec("a_family",		m_npcProtoList[i].m_family);
		dbNpc.GetRec("a_hit",			m_npcProtoList[i].m_nHit);
		dbNpc.GetRec("a_dodge",			m_npcProtoList[i].m_nDodge);
		dbNpc.GetRec("a_magicavoid",	m_npcProtoList[i].m_nMagicAvoid);

		dbNpc.GetRec("a_attribute",		npcattr);
		m_npcProtoList[i].m_attratt = GET_AT_ATT(npcattr);
		m_npcProtoList[i].m_attrdef = GET_AT_DEF(npcattr);

#ifdef EXTREME_CUBE
		dbNpc.GetRec("a_job_attribute",	m_npcProtoList[i].m_jobAttr);
#endif // EXTREME_CUBE

		//rvr
		dbNpc.GetRec("a_rvr_value", m_npcProtoList[i].m_rvr_value);
		dbNpc.GetRec("a_rvr_grade", m_npcProtoList[i].m_rvr_grade);
		//rvr

#if defined(LC_KOR)
		if( gserver->m_subno != 1 && npcIndex == 893 )		// 초보지원 NPC 1채널만
		{
			continue;
		}
#elif defined (LC_USA) || defined (LC_BILA) || defined (LC_GAMIGO) || defined (LC_RUS)
		if( gserver->m_subno != 2 && npcIndex == 893 )		// 초보지원 NPC 2채널만
		{
			continue;
		}
#else
		if( gserver->m_subno != 3 && npcIndex == 893 )		// 초보지원 NPC 3채널만
		{
			continue;
		}
#endif // defined (LC_KOR)

		if(gserver->m_subno != WAR_GROUND_CHANNEL && npcIndex == 1254)
		{
			continue;
		}

		// 공성 관련 NPC는 경험치, 나스, 스킬포인트 없음
		if (m_npcProtoList[i].m_flag & (NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
		{
			m_npcProtoList[i].m_price = 0;
			m_npcProtoList[i].m_exp = 0;
			m_npcProtoList[i].m_skillPoint = 0;
		}

		CItemProto* itemproto = NULL;
		// 공성 관련 npc는 아이템 드롭 없음
		m_npcProtoList[i].m_itemCount = 0;
		int k = 0;
		for(j = 0; j < MAX_NPC_DROPITEM && !(m_npcProtoList[i].m_flag & (NPC_CASTLE_GUARD | NPC_CASTLE_TOWER)); j++)
		{
			dbNpc.GetRec(a_item_str[j].c_str(),	m_npcProtoList[i].m_item[j-k]);

			if (m_npcProtoList[i].m_item[j-k] == -1 )
				break;

			itemproto = gserver->m_itemProtoList.FindIndex(m_npcProtoList[i].m_item[j-k]);
			dbNpc.GetRec(a_item_percent_str[j].c_str(), 	m_npcProtoList[i].m_itemPercent[j-k]);

			// 일본 50레벨 이상 무기, 방어구는 드롭확률 1/2로
#if defined (LC_TLD)
			if( itemproto )
			{
				if( itemproto->GetItemProtoLevel() >= 50 && (itemproto->getItemTypeIdx() == 0|| itemproto->getItemTypeIdx() == 1) )
				{
					m_npcProtoList[i].m_itemPercent[j-k] /= 2;
				}
			}
#endif
			m_npcProtoList[i].m_itemCount++;
		}

		dbNpc.GetRec("a_move_area",		m_npcProtoList[i].m_moveArea);
		dbNpc.GetRec("a_attack_area",	m_npcProtoList[i].m_attackArea);
		dbNpc.GetRec("a_str",			m_npcProtoList[i].m_str);
		dbNpc.GetRec("a_dex",			m_npcProtoList[i].m_dex);
		dbNpc.GetRec("a_int",			m_npcProtoList[i].m_int);
		dbNpc.GetRec("a_con",			m_npcProtoList[i].m_con);
		dbNpc.GetRec("a_hp",			m_npcProtoList[i].m_hp);
		dbNpc.GetRec("a_mp",			m_npcProtoList[i].m_mp);
		dbNpc.GetRec("a_attackSpeed",	m_npcProtoList[i].m_attackSpeed);
		dbNpc.GetRec("a_attackType",	m_npcProtoList[i].m_attackType);
		dbNpc.GetRec("a_attack",		m_npcProtoList[i].m_attack);
		dbNpc.GetRec("a_magic",			m_npcProtoList[i].m_magic);
		dbNpc.GetRec("a_defense",		m_npcProtoList[i].m_defense);
		dbNpc.GetRec("a_resist",		m_npcProtoList[i].m_resist);
		dbNpc.GetRec("a_attacklevel",	m_npcProtoList[i].m_attacklevel);
		dbNpc.GetRec("a_defenselevel",	m_npcProtoList[i].m_defenselevel);

// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
		dbNpc.GetRec("a_minplus",		m_npcProtoList[i].m_minplus);
		dbNpc.GetRec("a_maxplus",		m_npcProtoList[i].m_maxplus);
		dbNpc.GetRec("a_probplus",		m_npcProtoList[i].m_probplus);
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기

		// NPC AI
		dbNpc.GetRec("a_aitype",		m_npcProtoList[i].m_aitype);
		dbNpc.GetRec("a_aiflag",		m_npcProtoList[i].m_aiflag);
		dbNpc.GetRec("a_aileader_flag",	m_npcProtoList[i].m_aileader_flag);
		dbNpc.GetRec("a_ai_summonHp",	m_npcProtoList[i].m_ai_summonHp);
		dbNpc.GetRec("a_aileader_idx",	m_npcProtoList[i].m_aileader_idx);
		dbNpc.GetRec("a_aileader_count",m_npcProtoList[i].m_aileader_count);

		// 크기조정 : 스케일에 맞춰서
		dbNpc.GetRec("a_scale",			scale);
		dbNpc.GetRec("a_zone_flag",		m_npcProtoList[i].m_zone_flag);
		dbNpc.GetRec("a_extra_flag",	m_npcProtoList[i].m_extra_flag);

		dbNpc.GetRec("a_bound",			m_npcProtoList[i].m_bound);
		dbNpc.GetRec("a_lifetime",		m_npcProtoList[i].m_lifetime);

		// 보석 드롭 확률
		for( j = 0; j < MAX_NPC_DROPITEM; j++ )
		{
			dbNpc.GetRec(a_jewel_str[j].c_str(),	m_npcProtoList[i].m_jewel[j]);

			if (m_npcProtoList[i].m_item[j] == -1 )
				break;

			dbNpc.GetRec(a_jewel_percent_str[j].c_str(), m_npcProtoList[i].m_jewelPercent[j]);

			m_npcProtoList[i].m_jewelCount++;
		}

		m_npcProtoList[i].m_size *= scale;
		m_npcProtoList[i].m_attackArea *= scale;
		m_npcProtoList[i].m_bound *= scale;
		// ---

// --- 050311 : bs : 대만/중국에서 바알은 체력 회복이 3배
		for (j=0; j < MAX_NPC_PRODUCT; j++)
		{
			dbNpc.GetRec(a_product_str[j].c_str(), m_npcProtoList[i].m_product[j]);
		}

		// 스킬 입력
		CLCString npcskill(256);
		for (j = 0; j < MAX_NPC_SKILL; j++)
		{
			char tmpBuf[MAX_STRING_LENGTH] = {0,};
			if (dbNpc.GetRec(a_skill_str[j].c_str(), npcskill))
			{
				const char* p = npcskill;
				p = AnyOneArg(p, tmpBuf);
				if (strlen(tmpBuf) && atoi(tmpBuf) > 0)
				{
					m_npcProtoList[i].m_skillIndex[j] = atoi(tmpBuf);
					p = AnyOneArg(p, tmpBuf);
					m_npcProtoList[i].m_skillLevel[j] = atoi(tmpBuf);
					p = AnyOneArg(p, tmpBuf);
					m_npcProtoList[i].m_skillProb[j] = atoi(tmpBuf);
				}
				else
				{
					m_npcProtoList[i].m_skillIndex[j] = -1;
					m_npcProtoList[i].m_skillLevel[j] = 0;
					m_npcProtoList[i].m_skillProb[j] = 0;
				}
			}
		}
		CDBCmd jobitem;
		jobitem.Init(&gserver->m_dbdata);
		std::string select_npc_dorpjob_query = boost::str(boost::format("SELECT * FROM t_npc_dropjob WHERE a_npc_idx=%d limit 1") % npcIndex);
		jobitem.SetQuery(select_npc_dorpjob_query);
		if(jobitem.Open() && jobitem.MoveFirst())
		{
			if(jobitem.m_nrecords > 0)
			{
				int item[JOBCOUNT];
				int prob[JOBCOUNT];
				int jloop;
				jobitem.GetRec("a_titan_item", item[0]);
				jobitem.GetRec("a_titan_item_prob", prob[0]);
				jobitem.GetRec("a_knight_item", item[1]);
				jobitem.GetRec("a_knight_item_prob", prob[1]);
				jobitem.GetRec("a_healer_item",item[2]);
				jobitem.GetRec("a_healer_item_prob",prob[2]);
				jobitem.GetRec("a_mage_item",item[3]);
				jobitem.GetRec("a_mage_item_prob",prob[3]);
				jobitem.GetRec("a_rogue_item",item[4]);
				jobitem.GetRec("a_rogue_item_prob",prob[4]);
				jobitem.GetRec("a_sorcerer_item",item[5]);
				jobitem.GetRec("a_sorcerer_item_prob",prob[5]);
				jobitem.GetRec("a_nightshadow_item",item[6]);
				jobitem.GetRec("a_nightshadow_item_prob",prob[6]);
#ifdef EX_ROGUE
				jobitem.GetRec("a_exrogue_item",item[7]);
				jobitem.GetRec("a_exrogue_item_prob",prob[7]);
#endif // EX_ROGUE
#ifdef EX_MAGE
				jobitem.GetRec("a_exmage_item", item[8]);
				jobitem.GetRec("a_exmage_item_prob", prob[8]);
#endif // EX_MAGE
				for(jloop = 0; jloop < JOBCOUNT; jloop++)
				{
					m_npcProtoList[i].m_jobdropitem[jloop] = item[jloop];
					m_npcProtoList[i].m_jobdropitemprob[jloop] = prob[jloop];
				}
			}
		}

		CDBCmd allitem;
		allitem.Init(&gserver->m_dbdata);

		std::string select_npc_dorp_all_query = boost::str(boost::format("SELECT * FROM t_npc_drop_all WHERE a_npc_idx=%d") % npcIndex);
		allitem.SetQuery(select_npc_dorp_all_query);
		if(allitem.Open() && allitem.MoveFirst())
		{
			if(allitem.m_nrecords > 0)
			{
				int loopi = 0;
				do
				{
					allitem.GetRec("a_item_idx", m_npcProtoList[i].m_dropallitem[loopi]);
					allitem.GetRec("a_prob", m_npcProtoList[i].m_dropallitemprob[loopi]);
					loopi++;
					if(loopi == 20)
						break;
				}
				while(allitem.MoveNext());
			}
		}

		map_.insert(map_t::value_type(m_npcProtoList[i].m_index, &m_npcProtoList[i]));
		i++;
	}

	m_nCount = i;

	std::string select_npc_dorpaid_query = "";
	for (i = 0; i < m_nCount; i++)
	{
		select_npc_dorpaid_query = boost::str(boost::format("SELECT * FROM t_npc_dropraid WHERE a_npc_index=%d ORDER BY a_item_index") % m_npcProtoList[i].m_index);
		dbNpc.SetQuery(select_npc_dorpaid_query);
		if (!dbNpc.Open())
			return false;

		RAIDDROPDATA rdd;
		while (dbNpc.MoveNext())
		{
			dbNpc.GetRec("a_item_index", rdd.itemindex);
			dbNpc.GetRec("a_flag", rdd.flag);
			dbNpc.GetRec("a_count", rdd.count);
			dbNpc.GetRec("a_prob", rdd.prob);
#ifdef BUGFIX_SPECIALBOX_DROPRATE
			dbNpc.GetRec("a_spec_item_index1"	, rdd.spec_itemindex[0]);
			dbNpc.GetRec("a_spec_item_index2"	, rdd.spec_itemindex[1]);
			dbNpc.GetRec("a_spec_item_index3"	, rdd.spec_itemindex[2]);
			dbNpc.GetRec("a_spec_item_index4"	, rdd.spec_itemindex[3]);
			dbNpc.GetRec("a_spec_item_index5"	, rdd.spec_itemindex[4]);
			dbNpc.GetRec("a_spec_item_index6"	, rdd.spec_itemindex[5]);
			dbNpc.GetRec("a_spec_item_index7"	, rdd.spec_itemindex[6]);
			dbNpc.GetRec("a_spec_item_index8"	, rdd.spec_itemindex[7]);
			dbNpc.GetRec("a_spec_item_index9"	, rdd.spec_itemindex[8]);
			dbNpc.GetRec("a_spec_item_index10"	, rdd.spec_itemindex[9]);
			dbNpc.GetRec("a_spec_item_index11"	, rdd.spec_itemindex[10]);
			dbNpc.GetRec("a_spec_item_index12"	, rdd.spec_itemindex[11]);
			dbNpc.GetRec("a_spec_item_index13"	, rdd.spec_itemindex[12]);
			dbNpc.GetRec("a_spec_item_index14"	, rdd.spec_itemindex[13]);
#else //BUGFIX_SPECIALBOX_DROPRATE
			dbNpc.GetRec("a_spec_item_index1"	, rdd.spec_itemindex1);
			dbNpc.GetRec("a_spec_item_index2"	, rdd.spec_itemindex2);
			dbNpc.GetRec("a_spec_item_index3"	, rdd.spec_itemindex3);
			dbNpc.GetRec("a_spec_item_index4"	, rdd.spec_itemindex4);
			dbNpc.GetRec("a_spec_item_index5"	, rdd.spec_itemindex5);
			dbNpc.GetRec("a_spec_item_index6"	, rdd.spec_itemindex6);
			dbNpc.GetRec("a_spec_item_index7"	, rdd.spec_itemindex7);
			dbNpc.GetRec("a_spec_item_index8"	, rdd.spec_itemindex8);
			dbNpc.GetRec("a_spec_item_index9"	, rdd.spec_itemindex9);
			dbNpc.GetRec("a_spec_item_index10"	, rdd.spec_itemindex10);
			dbNpc.GetRec("a_spec_item_index11"	, rdd.spec_itemindex11);
			dbNpc.GetRec("a_spec_item_index12"	, rdd.spec_itemindex12);
			dbNpc.GetRec("a_spec_item_index13"	, rdd.spec_itemindex13);
			dbNpc.GetRec("a_spec_item_index14"	, rdd.spec_itemindex14);
#endif //BUGFIX_SPECIALBOX_DROPRATE
			dbNpc.GetRec("a_spec_min"			, rdd.spec_Min);
			dbNpc.GetRec("a_spec_max"			, rdd.spec_Max);
			dbNpc.GetRec("a_spec_count"			, rdd.spec_count);
			dbNpc.GetRec("a_spec_prob"			, rdd.spec_prob);
			dbNpc.GetRec("a_spec_flag"			, rdd.spec_flag);

			m_npcProtoList[i].m_listRaidDrop.push_back(rdd);
		}
	}

	return true;
}

CNPC* CNPCProtoList::Create(int npcindex, CNPCRegenInfo* regenInfo)
{
	CNPCProto* proto = FindProto(npcindex);
	if (!proto)
	{
		GAMELOG << init("SYS_ERR")
				<< "Can't Create NPC Num" << delim
				<< npcindex
				<< end;
		return NULL;
	}

	CNPC* npc = new CNPC;

	npc->m_regenInfo = regenInfo;

	// CNPC 속성
	npc->m_proto		= proto;
	npc->m_idNum		= proto->m_index;
	npc->m_moveArea		= proto->m_moveArea;
	npc->m_flag			= proto->m_flag;

	npc->ResetStat();

	// CCharacter 속성
	npc->m_index		= m_virtualIndex++;
	npc->m_name			= proto->m_name;
	npc->m_level		= proto->m_level;
	npc->m_exp			= proto->m_exp;
	npc->InitPointsToMax();		// 초기 최대 HP, MP 셋팅
	npc->m_skillPoint	= proto->m_skillPoint;

	npc->m_moveDir		= (GetRandom(0, 1) == 0) ? -1 : 1;

	// 퀘스트 NPC일 경우
	if (npc->m_proto->m_flag & NPC_QUEST)
	{
		int i;
		for (i=0; i < gserver->m_questProtoList.m_nCount; i++)
		{
			CQuestProto* questProto = gserver->m_questProtoList.m_proto + i;

			if (!questProto)
				continue;

			if (questProto->m_startType == QSTART_NPC  && questProto->m_startData == npc->m_idNum)
			{
				npc->m_quest[npc->m_nQuestCount] = questProto->m_index;
				npc->m_nQuestCount++;
			}
		}
	}

	// 050221 : bs : NPC 스킬 생성
	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (proto->m_skillIndex[i] > 0)
		{
			npc->m_skills[i] = gserver->m_skillProtoList.Create(proto->m_skillIndex[i], proto->m_skillLevel[i]);
		}
	}
	// --- 050221 : bs : NPC 스킬 생성

#ifdef SYSTEM_TREASURE_MAP
	npc->m_regenTime = gserver->getNowSecond(); // 생성 시간을 저장 한다.
#endif

	if (npc->m_proto->m_lifetime > 0)
	{
		npc->m_lifetime = npc->m_proto->m_lifetime + gserver->getNowSecond();
	}

	return npc;
}

// 리스트에서 찾기
CNPCProto* CNPCProtoList::FindProto(int npc_idx)
{
	map_t::iterator it = map_.find(npc_idx);
	return (it != map_.end()) ? it->second : NULL;
}
//
