#include <string>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "Descriptor.h"
#include "Character.h"
#include "gameserver_config.h"

#include "DBManager.h"

const int MAX_DB_QUERY = 8192;

//////////////////////////////////////////////////////////////////////////
void DBManager::_SaveCharaterQuery(std::vector<std::string> & vec, CPC* pChar, int table_no)
{
	char temp_nick_name[256] = {0,};
	mysql_real_escape_string(&gserver->m_dbchar, temp_nick_name, (const char *)pChar->m_nick, pChar->m_nick.Length());

	std::string updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery = "UPDATE t_characters SET ";
	updateQuery += boost::str(boost::format(" a_nick = '%s'") % temp_nick_name);
	updateQuery += boost::str(boost::format(", a_exp = %1%") % pChar->m_exp);
	updateQuery += boost::str(boost::format(", a_str = %d") % pChar->m_dbStr);
	updateQuery += boost::str(boost::format(", a_dex=%d") % pChar->m_dbDex);
	updateQuery += boost::str(boost::format(", a_int=%d") % pChar->m_dbInt);
	updateQuery += boost::str(boost::format(", a_con=%d") % pChar->m_dbCon);
	updateQuery += boost::str(boost::format(", a_statpt_remain=%d") % pChar->m_statpt_remain);
	updateQuery += boost::str(boost::format(", a_statpt_str=%d") % pChar->m_statpt_str);
	updateQuery += boost::str(boost::format(", a_statpt_dex=%d") % pChar->m_statpt_dex);
	updateQuery += boost::str(boost::format(", a_statpt_con=%d") % pChar->m_statpt_con);
	updateQuery += boost::str(boost::format(", a_statpt_int=%d") % pChar->m_statpt_int);
	updateQuery += boost::str(boost::format(", a_hp=%d") % pChar->m_hp);
	updateQuery += boost::str(boost::format(", a_max_hp=%d") % pChar->m_dbHP);
	updateQuery += boost::str(boost::format(", a_mp=%d") % pChar->m_mp);
	updateQuery += boost::str(boost::format(", a_max_mp=%d") % pChar->m_dbMP);
	updateQuery += boost::str(boost::format(", a_loseexp=%1%") % pChar->m_loseexp);
	updateQuery += boost::str(boost::format(", a_losesp=%1%") % pChar->m_losesp);
	updateQuery += boost::str(boost::format(", a_job2=%d") % (int)pChar->m_job2);

#ifdef REFORM_PK_PENALTY_201108
	updateQuery += boost::str(boost::format(", a_lcdatestamp=%d") % gserver->getNowSecond());
#endif

#ifdef ENABLE_SUBJOB
	updateQuery += boost::str(boost::format(", a_subjob=%d") % pChar->m_jobSub);
#endif //ENABLE_SUBJOB

	updateQuery += boost::str(boost::format(", a_hair_style=%d") % (int)pChar->m_hairstyle);
	updateQuery += boost::str(boost::format(", a_level=%d") % pChar->m_level);

	updateQuery += boost::str(boost::format(", a_skill_point=%1%") % pChar->m_skillPoint);

	updateQuery += boost::str(boost::format(", a_blood_point=%d") % pChar->m_bloodPoint);

	updateQuery += ", a_datestamp=now()";

	updateQuery += boost::str(boost::format(", a_was_x=%0.4f") % GET_X(pChar));
	updateQuery += boost::str(boost::format(", a_was_z=%0.4f") % GET_Z(pChar));
	updateQuery += boost::str(boost::format(", a_was_h=%0.4f") % GET_H(pChar));
	updateQuery += boost::str(boost::format(", a_was_r=%0.4f") % GET_R(pChar));
	updateQuery += boost::str(boost::format(", a_was_zone=%d") % ((pChar->m_pZone) ? pChar->m_pZone->m_index : -1) );
	updateQuery += boost::str(boost::format(", a_was_area=%d") % ((pChar->m_pArea) ? pChar->m_pArea->m_index : -1) );
	updateQuery += boost::str(boost::format(", a_was_yLayer=%d") % (int)GET_YLAYER(pChar));
	updateQuery += boost::str(boost::format(", a_silence_pulse=%d") % ((pChar->m_silencePulse - gserver->m_pulse) < 0 ? 0 : pChar->m_silencePulse - gserver->m_pulse) );
	updateQuery += boost::str(boost::format(", a_pkpenalty=%d") % pChar->m_pkPenalty);
	updateQuery += boost::str(boost::format(", a_pkcount=%d") % pChar->m_pkCount);
	updateQuery += boost::str(boost::format(", a_pkrecover=%d") % pChar->m_pkRecoverPulse);
	updateQuery += boost::str(boost::format(", a_pkpenaltyhp=%d") % pChar->m_pkPenaltyHP);
	updateQuery += boost::str(boost::format(", a_pkpenaltymp=%d") % pChar->m_pkPenaltyMP);
	updateQuery += boost::str(boost::format(", a_guildoutdate=%d") % pChar->m_guildoutdate);
	updateQuery += boost::str(boost::format(", a_pluseffect_option=%d") % (int)pChar->m_plusEffect);
	updateQuery += boost::str(boost::format(", a_fame=%d") % pChar->m_fame);

	updateQuery += boost::str(boost::format(", a_nas=%1%") % pChar->m_inventory.getMoney());

	std::string active_skill_index_list;
	pChar->m_activeSkillList.GetIndexString(active_skill_index_list);
	updateQuery += boost::str(boost::format(", a_active_skill_index='%s'") % active_skill_index_list);

	std::string active_skill_level_list;
	pChar->m_activeSkillList.GetLevelString(active_skill_level_list);
	updateQuery += boost::str(boost::format(", a_active_skill_level='%s'") % active_skill_level_list);

	std::string passive_skill_index_list;
	pChar->m_passiveSkillList.GetIndexString(passive_skill_index_list);
	updateQuery += boost::str(boost::format(", a_passive_skill_index='%s'") % passive_skill_index_list);

	std::string passive_skill_level_list;
	pChar->m_passiveSkillList.GetLevelString(passive_skill_level_list);
	updateQuery += boost::str(boost::format(", a_passive_skill_level='%s'") % passive_skill_level_list);

	std::string etc_skill_index_list;
	pChar->m_etcSkillList.GetIndexString(etc_skill_index_list);
	updateQuery += boost::str(boost::format(", a_etc_skill_index='%s'") % etc_skill_index_list);

	std::string etc_skill_level_list;
	pChar->m_etcSkillList.GetLevelString(etc_skill_level_list);
	updateQuery += boost::str(boost::format(", a_etc_skill_level='%s'") % etc_skill_level_list);

	std::string factory_skill_idx = "";
	std::string factory_skill_exp = "";
	for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
	{
		if (pChar->m_sealSkillExp[i].nSkillIdx < 0)
			continue;

		factory_skill_idx += boost::str(boost::format(" %d") % pChar->m_sealSkillExp[i].nSkillIdx);
		factory_skill_exp += boost::str(boost::format(" %d") % pChar->m_sealSkillExp[i].llExp);
	}

	updateQuery += boost::str(boost::format(", a_seal_skill_index='%s'") % factory_skill_idx);
	updateQuery += boost::str(boost::format(", a_seal_skill_exp='%s'") % factory_skill_exp);

	std::string costume = "";

	for (int i = 0; i < DEFAULT_MAX_WEARING; i++)
	{
		if(i < MAX_COSTUME_WEARING)
		{
			if( pChar->m_wearInventory.wearItemInfo[i+COSTUME2_WEARING_START] &&
				(pChar->m_wearInventory.wearItemInfo[i+COSTUME2_WEARING_START]->m_itemProto->getItemFlag() & ITEM_FLAG_INVISIBLE_COSTUME) == false &&
				i+COSTUME2_WEARING_START >= 20 && 
				i+COSTUME2_WEARING_START <= 27 )
			{
				costume += boost::str(boost::format(" %d") % pChar->m_wearInventory.wearItemInfo[i+COSTUME2_WEARING_START]->getDBIndex());
			}
			else if(pChar->m_wearInventory.wearItemInfo[i])
			{
				costume += boost::str(boost::format(" %d") % pChar->m_wearInventory.wearItemInfo[i]->getDBIndex());
			}
			else
			{
				costume += " -1";
			}
		}
		else
		{
			if(pChar->m_wearInventory.wearItemInfo[i])
				costume += boost::str(boost::format(" %d") % pChar->m_wearInventory.wearItemInfo[i]->getDBIndex());
			else
				costume += " -1";
		}
	}

	for (int i = 0; i < DEFAULT_MAX_WEARING; i++)
	{
		if(pChar->m_wearInventory.wearItemInfo[i])
			costume += boost::str(boost::format(" %d") % pChar->m_wearInventory.wearItemInfo[i]->getPlus());
		else
			costume += " 0";
	}

	updateQuery += boost::str(boost::format(", a_wearing='%s'") % costume);

	// Special Skill Save
	std::string specialSkill = "";
	pChar->m_sSkillList.GetString(specialSkill);
	updateQuery += boost::str(boost::format(", a_sskill='%s'") % specialSkill);

	// Quest Save
	updateQuery += boost::str(boost::format(", a_quest_index='', a_quest_value='', a_quest_complete='', a_quest_abandon=''"));

	updateQuery += boost::str(boost::format(", a_etc_event=%d") % pChar->m_etcEvent);

	updateQuery += boost::str(boost::format(", a_phoenix=%d") % pChar->m_bPhoenix_Char);
	updateQuery += boost::str(boost::format(", a_title_index=%d") % pChar->m_nCurrentTitle);
	updateQuery += boost::str(boost::format(", a_newtuto_complete=%d") % pChar->m_newtutoComplete);

	updateQuery += boost::str(boost::format(", a_exp_weekly=%d") % pChar->m_exp_weekly);

	updateQuery += boost::str(boost::format(", a_attendance_assure=%d") % pChar->m_attendanceManager.getUseAssure());

	updateQuery += boost::str(boost::format(" WHERE a_index=%d") % pChar->m_index);

	vec.push_back(updateQuery);
}

void DBManager::_SaveGuildPointQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery	= boost::str(boost::format(
								 "UPDATE t_characters_guildpoint SET a_guild_exp = %d, a_guild_fame = %d WHERE a_char_index = %d LIMIT 1") % pChar->m_GuildExp % pChar->m_GuildFame % pChar->m_index);

	vec.push_back(updateQuery);
}

void DBManager::_SaveQuestQuery(std::vector<std::string> & vec, CPC* pChar, int table_no)
{
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_questdata%02d WHERE a_char_index=%d") % table_no % pChar->m_index);
	vec.push_back(deleteQuery);

	if (pChar->m_questList.GetNextQuest(NULL) == NULL)
		return;

	bool saveflag = false;
	std::string insertQeury;
	insertQeury.reserve(MAX_DB_QUERY);
	insertQeury = boost::str(boost::format(
								 "INSERT INTO t_questdata%02d (a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2,a_quest_failvalue,a_complete_time) VALUES")
							 % table_no);

	CQuest* pQuest;
	CQuest* pQuestNext = pChar->m_questList.GetNextQuest(NULL);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pChar->m_questList.GetNextQuest(pQuestNext);

		saveflag = true;
		insertQeury += boost::str(boost::format(
									  "(%d, %d, %d, %d, %d, %d, %d, %d),") % pChar->m_index % pQuest->GetQuestIndex()
								  % (int)pQuest->GetQuestState() % pQuest->GetQuestValue(0) % pQuest->GetQuestValue(1)
								  % pQuest->GetQuestValue(2) % pQuest->GetFailValue()	% pQuest->GetCompleteTime() );
	}

	if (saveflag == false)
		return;

	int pos = insertQeury.rfind(",");
	insertQeury.erase(pos);

	vec.push_back(insertQeury);
}

void DBManager::_SaveAffinityQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteAffinityKeepQuery = boost::str(boost::format(
			"DELETE FROM t_affinity_keep WHERE a_charidx = %d") %pChar->m_index);

	vec.push_back(deleteAffinityKeepQuery);


	std::string deleteAffinityRewardStepQuery = boost::str(boost::format(
				"DELETE FROM t_affinity_reward_step WHERE a_charidx = %d") %pChar->m_index);

	vec.push_back(deleteAffinityRewardStepQuery);

	if (pChar->m_affinityList.GetList()->empty() == false)
	{
		bool affinityKeepInsertQuery_saveflag = false;
		std::string affinityKeepInsertQuery;
		affinityKeepInsertQuery.reserve(MAX_DB_QUERY);
		affinityKeepInsertQuery = "INSERT INTO t_affinity_keep (`a_charidx`, `a_affinity_idx`, `a_point`) VALUES";

		bool affinityRewardStepInsertQuery_saveflag = false;
		std::string affinityRewardStepInsertQuery;
		affinityRewardStepInsertQuery.reserve(MAX_DB_QUERY);
		affinityRewardStepInsertQuery = "INSERT INTO t_affinity_reward_step (`a_charidx`, `a_npcidx`, `a_reward_step`) VALUES";

		map_affinitylist_t* list = pChar->m_affinityList.GetList();
		map_affinitylist_t::iterator it = list->begin();
		map_affinitylist_t::iterator endit = list->end();
		for( ; it != endit; ++it )
		{
			CAffinity* data = it->second;

			affinityKeepInsertQuery_saveflag = true;
			affinityKeepInsertQuery += boost::str(boost::format("(%d, %d, %d),") %pChar->m_index % data->m_affinityidx % data->m_point);

			set_rewardstep_t::iterator it = data->m_rewardstep.begin();
			set_rewardstep_t::iterator endit = data->m_rewardstep.end();
			for(; it != endit; ++it)
			{
				REWARDSTEP* rewardstep = *(it);

				affinityRewardStepInsertQuery_saveflag = true;
				affinityRewardStepInsertQuery += boost::str(boost::format("(%d, %d, %d),") % pChar->m_index % rewardstep->affinitynpc % rewardstep->point);
			}
		}

		if (affinityKeepInsertQuery_saveflag)
		{
			int pos = affinityKeepInsertQuery.rfind(",");
			affinityKeepInsertQuery.erase(pos);
			vec.push_back(affinityKeepInsertQuery);
		}

		if (affinityRewardStepInsertQuery_saveflag)
		{
			int pos = affinityRewardStepInsertQuery.rfind(",");
			affinityRewardStepInsertQuery.erase(pos);
			vec.push_back(affinityRewardStepInsertQuery);
		}
	}
}

void DBManager::_SaveAssistQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_assist WHERE a_char_index= %d") % pChar->m_index);

	vec.push_back(deleteQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = "INSERT INTO t_assist "
				  " (a_char_index, a_help_item, a_help_skill, a_help_level, a_help_remain"
				  ", a_help_remaincount"
				  ", a_help_hit0, a_help_hit1, a_help_hit2, a_curse_item, a_curse_skill, a_curse_level, a_curse_remain"
				  ", a_curse_remaincount"
				  ", a_curse_hit0, a_curse_hit1, a_curse_hit2)"
				  " VALUES (";
	insertQuery += boost::str(boost::format("%d,") % pChar->m_index);

	std::string out_assist_item = "";
	std::string out_assist_skill = "";
	std::string out_assist_level = "";
	std::string out_assist_remain = "";
	std::string out_assist_remain_count = "";
	std::string out_assist_hit0 = "";
	std::string out_assist_hit1 = "";
	std::string out_assist_hit2 = "";

	pChar->m_assist.GetListString(true, out_assist_item, out_assist_skill, out_assist_level, out_assist_remain,
								  out_assist_remain_count,
								  out_assist_hit0, out_assist_hit1, out_assist_hit2);
	insertQuery += boost::str(boost::format("'%s'") % out_assist_item);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_skill);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_level);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_remain);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_remain_count);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit0);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit1);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit2);


	pChar->m_assist.GetListString(false, out_assist_item, out_assist_skill, out_assist_level, out_assist_remain,
								  out_assist_remain_count,
								  out_assist_hit0, out_assist_hit1, out_assist_hit2);

	insertQuery += boost::str(boost::format(", '%s'") % out_assist_item);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_skill);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_level);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_remain);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_remain_count);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit0);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit1);
	insertQuery += boost::str(boost::format(", '%s'") % out_assist_hit2);
	insertQuery += boost::str(boost::format(")"));

	vec.push_back(insertQuery);
}

void DBManager::_SaveAssistABSTimeQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_assist_abstime WHERE a_char_index=%d") % pChar->m_index);

	vec.push_back(deleteQuery);

	int nAssistABSTypeCount = pChar->m_assist.GetABSTimeTypeList(NULL);
	if (nAssistABSTypeCount > 0)
	{
		int* nAssistABSItemIndex = new int[nAssistABSTypeCount];
		int* nAssistABSSkillIndex = new int[nAssistABSTypeCount];
		int* nAssistABSSkillLevel = new int[nAssistABSTypeCount];
		char* nAssistABSHit0 = new char[nAssistABSTypeCount];
		char* nAssistABSHit1 = new char[nAssistABSTypeCount];
		char* nAssistABSHit2 = new char[nAssistABSTypeCount];
		int* nAssistABSEndTime = new int[nAssistABSTypeCount];

		pChar->m_assist.GetABSTimeTypeList(nAssistABSItemIndex, nAssistABSSkillIndex, nAssistABSSkillLevel, nAssistABSHit0, nAssistABSHit1, nAssistABSHit2, nAssistABSEndTime);

		std::string insertSql;
		insertSql.reserve(MAX_DB_QUERY);
		insertSql = "INSERT INTO t_assist_abstime (a_char_index, a_item_index, a_skill_index, a_skill_level, a_hit0, a_hit1, a_hit2, a_end_time) VALUES";

		for (int i = 0; i < nAssistABSTypeCount; i++)
		{
			insertSql += boost::str(boost::format(
										"( %d, %d, %d, %d, %d, %d, %d, %d),") % pChar->m_index %	nAssistABSItemIndex[i] % nAssistABSSkillIndex[i]
									% nAssistABSSkillLevel[i] % (int)nAssistABSHit0[i] % (int)nAssistABSHit1[i] % (int)nAssistABSHit2[i] %nAssistABSEndTime[i]);
		}

		int pos = insertSql.rfind(",");
		insertSql.erase(pos);

		vec.push_back(insertSql);

		delete [] nAssistABSItemIndex;
		delete [] nAssistABSSkillIndex;
		delete [] nAssistABSSkillLevel;
		delete [] nAssistABSHit0;
		delete [] nAssistABSHit1;
		delete [] nAssistABSHit2;
		delete [] nAssistABSEndTime;
	}
}

void DBManager::_SaveCharInvenQuery(std::vector<std::string> & vec, CPC* pChar,int table_no)
{
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_inven%02d WHERE a_char_idx=%d") % table_no % pChar->m_index);

	vec.push_back(deleteQuery);

	pChar->m_inventory.getSaveString(vec);
}

void DBManager::_SaveCharcterStashQuery(std::vector<std::string> & vec, CPC* pChar)
{
	int stashIndex = pChar->m_desc->m_index % 10;

	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_stash%02d WHERE a_user_idx=%d") % stashIndex % pChar->m_index);

	vec.push_back(deleteQuery);

	pChar->m_inventory.getSaveStringInStach(vec);
}

void DBManager::_SaveCharacterQuickSlotQuery(std::vector<std::string> & vec, CPC* pChar, int table_no)
{
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_quickslot%02d WHERE a_char_idx=%d") % table_no % pChar->m_index);

	vec.push_back(deleteQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format("INSERT INTO t_quickslot%02d (a_char_idx, a_page_idx, a_slot) VALUES") % table_no);

	for (int i = 0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		std::string tmpString = "";

		for (int slot = 0; slot < QUICKSLOT_MAXSLOT; slot++)
		{
			switch (pChar->m_quickSlot[i].m_slotType[slot])
			{
			case QUICKSLOT_TYPE_SKILL:
				tmpString += boost::str(boost::format("%d %d ") % pChar->m_quickSlot[i].m_slotType[slot] % pChar->m_quickSlot[i].m_skillType[slot]);
				break;

			case QUICKSLOT_TYPE_ACTION:
				tmpString += boost::str(boost::format("%d %d ") % pChar->m_quickSlot[i].m_slotType[slot] % pChar->m_quickSlot[i].m_actionType[slot]);
				break;

			case QUICKSLOT_TYPE_ITEM_WEAR:
				if (pChar->m_quickSlot[i].m_item[slot])
				{
					int WearItemType = -1;
					if(pChar->m_quickSlot[i].m_item[slot]->getWearPos() == COSTUME2_WEARING_SUIT)
						WearItemType = ITEM_COSTUM_SUIT;

					else if(pChar->m_quickSlot[i].m_item[slot]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
						WearItemType = ITEM_COSTUM;

					else
						WearItemType = ITEM_NORMAL;

					tmpString += boost::str(boost::format("%d %d %d ") % pChar->m_quickSlot[i].m_slotType[slot] % WearItemType % (int)pChar->m_quickSlot[i].m_item[slot]->getWearPos());
				}
				else
				{
					tmpString += "-1 ";
				}
				break;

			case QUICKSLOT_TYPE_ITEM:
				if (pChar->m_quickSlot[i].m_item[slot])
				{
					tmpString += boost::str(boost::format("%d %d %d ") % pChar->m_quickSlot[i].m_slotType[slot] % pChar->m_quickSlot[i].m_item[slot]->tab() % pChar->m_quickSlot[i].m_item[slot]->getInvenIndex());
				}
				else
				{
					tmpString += "-1 ";
				}
				break;

			case QUICKSLOT_TYPE_EMPTY:
			default:
				tmpString += "-1 ";
				break;
			}
		}

		insertQuery += boost::str(boost::format("(%d, %d,'%s'),") % pChar->m_index % i % tmpString );
	}

	int pos = insertQuery.rfind(",");
	insertQuery.erase(pos);

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterMemposQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string selectQuery = boost::str(boost::format(
			"SELECT a_char_idx FROM t_mempos WHERE a_char_idx=%d LIMIT 1") % pChar->m_index);

	vec.push_back(selectQuery);

	// MySQL 문자열 보호용 임시 데이터
	char tComment[MEMPOS_COMMENT_LENGTH * 2 + 1];

	// update query
	std::string updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery = "UPDATE t_mempos SET ";

	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (pChar->m_mempos.m_data[i])
		{
			mysql_real_escape_string(&gserver->m_dbchar, tComment, (const char *)pChar->m_mempos.m_data[i]->m_comment, pChar->m_mempos.m_data[i]->m_comment.Length());

			updateQuery += boost::str(boost::format("a_mem_%d ='%d %0.4f %0.4f %d %s',") % i % pChar->m_mempos.m_data[i]->m_zone
									  % pChar->m_mempos.m_data[i]->m_x % pChar->m_mempos.m_data[i]->m_z % (int)pChar->m_mempos.m_data[i]->m_ylayer % tComment);
		}
		else
		{
			updateQuery += boost::str(boost::format("a_mem_%d='',") % i );
		}
	}
	int updatePos = updateQuery.rfind(",");
	updateQuery.erase(updatePos);
	updateQuery += boost::str(boost::format(" WHERE a_char_idx=%d") % pChar->m_index);
	vec.push_back(updateQuery);

	// insert query
	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format(
								 "INSERT INTO t_mempos (a_char_idx, a_mem_0, a_mem_1, a_mem_2, a_mem_3, a_mem_4, a_mem_5, a_mem_6, a_mem_7, "
								 "a_mem_8, a_mem_9, a_mem_10, a_mem_11, a_mem_12, a_mem_13, a_mem_14) VALUES (%d") % pChar->m_index);

	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (pChar->m_mempos.m_data[i])
		{
			mysql_real_escape_string(&gserver->m_dbchar, tComment, (const char *)pChar->m_mempos.m_data[i]->m_comment, pChar->m_mempos.m_data[i]->m_comment.Length());

			insertQuery += boost::str(boost::format(", '%d %0.4f %0.4f %d %s'") % pChar->m_mempos.m_data[i]->m_zone
									  % pChar->m_mempos.m_data[i]->m_x % pChar->m_mempos.m_data[i]->m_z % (int)pChar->m_mempos.m_data[i]->m_ylayer % tComment);
		}
		else
		{
			insertQuery += boost::str(boost::format(",''"));
		}
	}

	insertQuery += boost::str(boost::format(")") );
	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterCashItemDateQuery(std::vector<std::string> & vec, CPC* pChar, int descriptorUserIndex)
{
	std::string selectQuery = boost::str(boost::format(
			"SELECT a_portal_idx FROM t_cashItemdate WHERE a_portal_idx = %d LIMIT 1") % descriptorUserIndex);
	vec.push_back(selectQuery);

	std::string updateQuery;
	updateQuery = boost::str(boost::format(
								 "UPDATE t_cashItemdate SET a_mempos_new = %d where a_portal_idx = %d") % pChar->m_memposTime % descriptorUserIndex);
	vec.push_back(updateQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format(
								 "INSERT INTO t_cashItemdate (a_portal_idx, a_mempos_new, a_charslot0_new, a_charslot1_new, a_stashext_new) VALUES ( %d, %d, 0, 0, 0 )") % descriptorUserIndex % pChar->m_memposTime);
	vec.push_back(insertQuery);

	std::string cashItemStashQuery;
	cashItemStashQuery = boost::str(boost::format(
										"UPDATE t_cashItemdate SET a_stashext_new = %d where a_portal_idx = %d") % pChar->m_stashextTime % descriptorUserIndex);

	vec.push_back(cashItemStashQuery);
}

void DBManager::_SaveCharacterPersonalDungeonQuery(std::vector<std::string> & vec, CPC* pChar, int table_no)
{
	std::string selectQuery = boost::str(boost::format(
			"SELECT a_char_index FROM t_pddata%02d WHERE a_char_index=%d LIMIT 1") % table_no % pChar->m_index);
	vec.push_back(selectQuery);

	std::string updateQuery;
	updateQuery = boost::str(boost::format(
								 "UPDATE t_pddata%02d SET a_pd3_count=%d, a_pd3_time=%d, a_pd4_count=%d, a_pd4_time=%d WHERE a_char_index=%d")
							 % table_no % pChar->m_pd3Count % pChar->m_pd3Time % pChar->m_pd4Count % pChar->m_pd4Time %pChar->m_index);
	vec.push_back(updateQuery);


	std::string insertQuery;
	insertQuery = boost::str(boost::format(
								 "INSERT INTO t_pddata%02d (a_char_index, a_pd3_count, a_pd3_time, a_pd4_count, a_pd4_time) VALUES (%d, %d, %d, %d, %d)")
							 % table_no % pChar->m_index % pChar->m_pd3Count % pChar->m_pd3Time % pChar->m_pd4Count %pChar->m_pd4Time);

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterPetPreQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string updateQuery;
	updateQuery = boost::str(boost::format(
								 "UPDATE t_pet SET a_enable=2 WHERE a_owner=%d") % pChar->m_index);
	vec.push_back(updateQuery);
}

void DBManager::_SaveCharacterPetQuery(std::vector<std::string> & vec, CPC* pChar, CPet* pet)
{
	// update / insert를 구분하기 위함
	std::string selectQuery = boost::str(boost::format(
			"SELECT a_index FROM t_pet WHERE a_index=%d AND a_owner=%d LIMIT 1") % pet->m_index % pChar->m_index);
	vec.push_back(selectQuery);

	int petTurnToNpc = pet->GetPetTurnToNpc();
	int petTurnToNpcSize = pet->GetPetTurnToNpcSize();
	if(!gserver->isActiveEvent(A_EVENT_XMAS) && petTurnToNpc == 485)
	{
		petTurnToNpc = 0;
		petTurnToNpcSize = 10;
	}
	std::string petSkillIndex ="";
	std::string petSkillLevel ="";
	pet->m_skillList.GetIndexString(petSkillIndex);
	pet->m_skillList.GetLevelString(petSkillLevel);

	std::string  updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery = "UPDATE t_pet SET ";
	updateQuery += boost::str(boost::format( "a_owner=%d") % pChar->m_index);
	updateQuery += boost::str(boost::format( ", a_enable=1") );
	updateQuery += boost::str(boost::format( ", a_lastupdate=%d") % gserver->getNowSecond());
	updateQuery += boost::str(boost::format( ", a_type=%d") % (int)pet->m_petTypeGrade);
	updateQuery += boost::str(boost::format( ", a_level=%d") % pet->m_level);
	updateQuery += boost::str(boost::format( ", a_hp=%d") % pet->m_hp);
	updateQuery += boost::str(boost::format( ", a_hungry=%d") % pet->m_hungry);
	updateQuery += boost::str(boost::format( ", a_sympathy=%d") % pet->m_sympathy);
	updateQuery += boost::str(boost::format( ", a_exp=%1%") % pet->m_exp);
	updateQuery += boost::str(boost::format( ", a_ability=%d") % pet->GetAbilityPoint());
	updateQuery += boost::str(boost::format( ", a_skill_index='%s'") % petSkillIndex);
	updateQuery += boost::str(boost::format( ", a_skill_level='%s'") % petSkillLevel);
	updateQuery += boost::str(boost::format( ", a_time_rebirth=%d") % pet->m_nRemainRebirth);
	updateQuery += boost::str(boost::format( ", a_color=%d") % (int)pet->m_petColorType);
	updateQuery += boost::str(boost::format( ", a_pet_turnto_npc=%d") % petTurnToNpc);
#ifdef PET_TURNTO_NPC_ITEM
	updateQuery += boost::str(boost::format( ", a_pet_size=%d") % petTurnToNpcSize);
#endif //PET_TURNTO_NPC_ITEM
	updateQuery += boost::str(boost::format( " WHERE a_index = %d AND a_owner = %d") % pet->m_index % pChar->m_index);

	vec.push_back(updateQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = "INSERT INTO t_pet (a_index, a_owner, a_enable, a_lastupdate,	a_type, a_level, a_hp, a_hungry, a_sympathy, a_exp, a_ability, a_skill_index, a_skill_level, a_time_rebirth, a_color, a_pet_turnto_npc";
#ifdef PET_TURNTO_NPC_ITEM
	insertQuery += boost::str(boost::format(", a_pet_size"));
#endif //PET_TURNTO_NPC_ITEM
	insertQuery += boost::str(boost::format(")"));
	insertQuery += boost::str(boost::format(" VALUES ("));
	insertQuery += boost::str(boost::format("%d") % pet->m_index);
	insertQuery += boost::str(boost::format(", %d") % pChar->m_index);
	insertQuery += boost::str(boost::format(", 1"));
	insertQuery += boost::str(boost::format(", %d") % gserver->getNowSecond());
	insertQuery += boost::str(boost::format(", %d") % (int)pet->m_petTypeGrade);
	insertQuery += boost::str(boost::format(", %d") % pet->m_level);
	insertQuery += boost::str(boost::format(", %d") % pet->m_hp);
	insertQuery += boost::str(boost::format(", %d") % pet->m_hungry);
	insertQuery += boost::str(boost::format(", %d") % pet->m_sympathy);
	insertQuery += boost::str(boost::format(", %1%") % pet->m_exp);
	insertQuery += boost::str(boost::format(", %d") % pet->m_abilityPoint);
	insertQuery += boost::str(boost::format(", '%s'") % petSkillIndex);
	insertQuery += boost::str(boost::format(", '%s'") % petSkillLevel);
	insertQuery += boost::str(boost::format(", %d") % pet->m_nRemainRebirth);
	insertQuery += boost::str(boost::format(", %d") % (int)pet->m_petColorType);
	insertQuery += boost::str(boost::format(", %d") % petTurnToNpc);
#ifdef PET_TURNTO_NPC_ITEM
	insertQuery += boost::str(boost::format(", %d") % petTurnToNpcSize);
#endif //PET_TURNTO_NPC_ITEM
	insertQuery += boost::str(boost::format(")") );

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterPetNameQuery(std::vector<std::string> & vec, CPC* pChar, CPet* pet)
{
	if( pet->m_name.Length() == 0 )
		return;

	char tpetname[128] = {0,};
	mysql_real_escape_string(&gserver->m_dbchar, tpetname, (const char *)pet->m_name, pet->m_name.Length());

	std::string selectQuery = boost::str(boost::format(
			"SELECT a_pet_index FROM t_pet_name WHERE a_pet_index=%d LIMIT 1") % pet->m_index);
	vec.push_back(selectQuery);

	std::string updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery = boost::str(boost::format(
								 "UPDATE t_pet_name SET a_pet_owner=%d, a_pet_name='%s' WHERE a_pet_index=%d") % pChar->m_index % tpetname % pet->m_index);
	vec.push_back(updateQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format(
								 "INSERT INTO t_pet_name ( a_pet_index, a_pet_owner, a_pet_name ) VALUES ( %d, %d, '%s' )") % pet->m_index % pChar->m_index % tpetname);
	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterPetTempDeleteQuery(std::vector<std::string> & vec, CPC* pChar)
{
	// 임시저장 애완동물 삭제 : a_enable == 2
	std::string deleteQuery = boost::str(boost::format(
			"DELETE FROM t_pet WHERE a_owner=%d AND a_enable=2") % pChar->m_index);

	vec.push_back(deleteQuery);
}

void DBManager::_SaveCharacterAttackPetPreQuery(std::vector<std::string> & vec, CPC* pChar)
{
	// 애완동물 상태를 2로 변경후에 지금 들고 있는 펫은 다시 1로 변경하면서 저장하고 나머지 삭제
	std::string updateQuery = boost::str(boost::format("UPDATE t_apets SET a_enable=2 WHERE a_owner=%d") % pChar->m_index);

	vec.push_back(updateQuery);
}

void DBManager::_SaveCharacterAttackPetQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet)
{
	{
		// update / insert를 구분하기 위해
		std::string str = boost::str(boost::format("SELECT a_index FROM t_apets WHERE a_index=%d AND a_owner=%d LIMIT 1") % apet->m_index % pChar->m_index);

		vec.push_back(str);
	}

	std::string petSkillIndex;
	apet->m_skillList.GetIndexString(petSkillIndex);

	std::string petSkillLevel;
	apet->m_skillList.GetLevelString(petSkillLevel);

	std::string passiveSkillIndex;
	std::string passiveSkillLevel;
	apet->m_passiveSkillList.GetIndexString(passiveSkillIndex);
	apet->m_passiveSkillList.GetLevelString(passiveSkillLevel);
	petSkillIndex += (" " + passiveSkillIndex);
	petSkillLevel += (" " + passiveSkillLevel);

	std::string updateQuery;
	updateQuery.reserve(MAX_DB_QUERY);
	updateQuery = "UPDATE t_apets SET ";

	updateQuery += boost::str(boost::format("a_proto_index=%d") % apet->m_pProto->Index());
	updateQuery += boost::str(boost::format(", a_enable=1"));
	updateQuery += boost::str(boost::format(", a_seal=%d") % apet->m_nSeal);
	updateQuery += boost::str(boost::format(", a_name='%s'") % (const char*)apet->m_name);
	updateQuery += boost::str(boost::format(", a_level=%d") % apet->m_level);
	updateQuery += boost::str(boost::format(", a_exp=%1%") % apet->m_exp);
	updateQuery += boost::str(boost::format(", a_remain_stat=%d") % apet->m_nRemainStat);
	updateQuery += boost::str(boost::format(", a_plus_str=%d") % apet->m_nPlusStr);
	updateQuery += boost::str(boost::format(", a_plus_con=%d") % apet->m_nPlusCon);
	updateQuery += boost::str(boost::format(", a_plus_dex=%d") % apet->m_nPlusDex);
	updateQuery += boost::str(boost::format(", a_plus_int=%d") % apet->m_nPlusInt);
	updateQuery += boost::str(boost::format(", a_hp=%d") % apet->m_hp);
	updateQuery += boost::str(boost::format(", a_mp=%d") % apet->m_mp);
	updateQuery += boost::str(boost::format(", a_skill_point=%d") % apet->m_nSP);
	updateQuery += boost::str(boost::format(", a_faith=%d") % apet->GetFaith());
	updateQuery += boost::str(boost::format(", a_stm=%d") % apet->GetStamina());
	updateQuery += boost::str(boost::format(", a_skill_level='%s'") % petSkillLevel);
	updateQuery += boost::str(boost::format(", a_skill_index='%s'") % petSkillIndex);
	updateQuery += boost::str(boost::format(", a_ai_enable=%d") % (int)apet->m_cAI);
	updateQuery += boost::str(boost::format(", a_ai_slot=%d") % apet->m_nAISlot);
	updateQuery += boost::str(boost::format(", a_accFaith=%d") % apet->m_nAccFaith);
	updateQuery += boost::str(boost::format(", a_accStm=%d") % apet->m_nAccStemina);
	updateQuery += boost::str(boost::format(", a_accExp=%1%") % apet->GetAccExp());
	updateQuery += boost::str(boost::format(", a_cooltime=%d") % apet->GetCooltime());
	updateQuery += boost::str(boost::format(" WHERE a_index=%d AND a_owner=%d") % apet->m_index % pChar->m_index);

	vec.push_back(updateQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = "INSERT INTO t_apets (a_index, a_owner, a_enable, a_seal, a_proto_index, a_name, a_create_date, a_lastupdate_date, a_level, a_exp, a_remain_stat, a_plus_str, a_plus_con, a_plus_dex, a_plus_int,"
				  " a_skill_point, a_skill_index, a_skill_level, a_hp, a_mp, a_faith, a_stm";
	insertQuery += ", a_ai_enable, a_ai_slot";
	insertQuery += " ,a_accFaith ,a_accStm";
	insertQuery += " ,a_accExp ,a_cooltime";
	insertQuery += ") VALUES (";
	insertQuery += boost::str(boost::format("%d") % apet->m_index);		// apet index
	insertQuery += boost::str(boost::format(",%d") % pChar->m_index);		// owner index
	insertQuery += ", 1";						// enable
	insertQuery += boost::str(boost::format(", %d") % apet->m_nSeal);		// seal
	insertQuery += boost::str(boost::format(", %d") % apet->m_pProto->Index());	// proto index
	insertQuery += boost::str(boost::format(", '%s'") % (const char*)apet->m_name);	// pet name
	insertQuery += ", NOW()";				// create date
	insertQuery += ", NOW()";				// last upate date
	insertQuery += boost::str(boost::format(", %d") % apet->m_level);		// level
	insertQuery += boost::str(boost::format(", %1%") % apet->m_exp);		// exp
	insertQuery += boost::str(boost::format(", %d") % apet->m_nRemainStat);		// remain stat
	insertQuery += boost::str(boost::format(", %d") % apet->m_nPlusStr);		// plus_str
	insertQuery += boost::str(boost::format(", %d") % apet->m_nPlusCon);		// plus_con
	insertQuery += boost::str(boost::format(", %d") % apet->m_nPlusDex);		// plus_dex
	insertQuery += boost::str(boost::format(", %d") % apet->m_nPlusInt);		// plus_int
	insertQuery += boost::str(boost::format(", %d") % apet->m_nSP);		// skill_point
	insertQuery += boost::str(boost::format(", '%s'") % petSkillIndex);		// skill_index
	insertQuery += boost::str(boost::format(", '%s'") % petSkillLevel);		// skill_level
	insertQuery += boost::str(boost::format(", %d") % apet->m_hp);		// hp
	insertQuery += boost::str(boost::format(", %d") % apet->m_mp);		// mp
	insertQuery += boost::str(boost::format(", %d") % apet->GetFaith());		// a_faith
	insertQuery += boost::str(boost::format(", %d") % apet->GetStamina());		// a_stm
	insertQuery += boost::str(boost::format(", %d") % (int)apet->m_cAI);		// a_ai_enalbe
	insertQuery += boost::str(boost::format(", %d") % apet->m_nAISlot);		// a_ai_slot
	insertQuery += boost::str(boost::format(", %d") % apet->m_nAccFaith);		// accFaith
	insertQuery += boost::str(boost::format(", %d") % apet->m_nAccStemina);		// accStm
	insertQuery += boost::str(boost::format(", %d") % apet->GetAccExp());		// accExp
	insertQuery += boost::str(boost::format(", %d") % apet->GetCooltime());		// cool time
	insertQuery += ")";

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterAttackPetInvenQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet)
{
	vec;
	pChar;
	apet;
	// 펫 인벤 4칸 저장 //////////////////////////////////////////////////////

	std::string deleteQuery = boost::str(boost::format("DELETE FROM t_apets_inven WHERE a_apet_idx=%d") % apet->m_index );
	vec.push_back(deleteQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = "INSERT INTO t_apets_inven (a_apet_idx"
				  ", a_item_idx0, a_plus0, a_wear_pos0, a_flag0, a_used0, a_used0_2, a_serial0, a_count0, a_item0_option0, a_item0_option1, a_item0_option2, a_item0_option3, a_item0_option4"
				  ", a_item_idx1, a_plus1, a_wear_pos1, a_flag1, a_used1, a_used1_2, a_serial1, a_count1, a_item1_option0, a_item1_option1, a_item1_option2, a_item1_option3, a_item1_option4"
				  ", a_item_idx2, a_plus2, a_wear_pos2, a_flag2, a_used2, a_used2_2, a_serial2, a_count2, a_item2_option0, a_item2_option1, a_item2_option2, a_item2_option3, a_item2_option4"
				  ", a_item_idx3, a_plus3, a_wear_pos3, a_flag3, a_used3, a_used3_2, a_serial3, a_count3, a_item3_option0, a_item3_option1, a_item3_option2, a_item3_option3, a_item3_option4"
				  ") VALUES (";

	insertQuery += boost::str(boost::format("%d") % apet->m_index);	//a_apet_idx
	insertQuery += boost::str(boost::format(","));


	for ( int i = 0; i < APET_WEARPOINT; i++)
	{
		if (apet->m_wearing[i] == NULL)
		{
			insertQuery += "-1, 0, -1, 0, -1, -1, 'NONE', 0, 0, 0, 0, 0, 0";	//a_apet_idx
		}
		else
		{
			insertQuery += boost::str(boost::format("%d") % apet->m_wearing[i]->getDBIndex());	//a_item_idx0
			insertQuery += boost::str(boost::format(", %d") % apet->m_wearing[i]->getPlus());	//a_plus0
			insertQuery += boost::str(boost::format(", %d") % (int)apet->m_wearing[i]->getWearPos());	//a_wear_pos0
			insertQuery += boost::str(boost::format(", %d") % apet->m_wearing[i]->getFlag());	//a_flag0
			insertQuery += boost::str(boost::format(", %d") % apet->m_wearing[i]->getUsed());	//a_used0
			insertQuery += boost::str(boost::format(", %d") % apet->m_wearing[i]->getUsed_2());	//a_used0_2
			insertQuery += boost::str(boost::format(", '%s'") % apet->m_wearing[i]->m_serial);	//a_serial0
			insertQuery += boost::str(boost::format(", %1%") % apet->m_wearing[i]->Count());	//a_count0

			if (apet->m_wearing[i]->IsRareItem())
			{
				if (apet->m_wearing[i]->m_nOption == 2)
				{
					insertQuery += boost::str(boost::format(", %d, %d") % apet->m_wearing[i]->m_nRareOptionIndex % apet->m_wearing[i]->m_nRareOptionBit);

					for ( int optionIndex = 2; optionIndex < MAX_ITEM_OPTION; optionIndex++)
					{
						insertQuery += ",0";
					}
				}
				else
				{
					insertQuery += ", 0, 0, 0, 0, 0";
				}
			}
			else
			{
				insertQuery += ", 0, 0, 0, 0, 0";
			}
		}
		if (i < APET_WEARPOINT - 1)
			insertQuery += ",";
	}
	insertQuery += ")";

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterAttackPetAIQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet)
{
	vec;
	pChar;
	apet;

	if (apet->m_nAICount)
	{

		// 모두 DELETE 후에 다시 INSERT를 한다.대신 로그로 지워진 정보는 남김.
		// 펫AI 저장 - DELETE
		std::string deleteQuery = boost::str(boost::format("DELETE FROM t_apets_ai WHERE a_char_idx = %d and a_apet_idx = %d") % pChar->m_index % apet->m_index);
		vec.push_back(deleteQuery);

		std::string insertQuery;
		insertQuery.reserve(MAX_DB_QUERY);
		insertQuery = boost::str(boost::format(
									 "INSERT INTO t_apets_ai (a_char_idx, a_apet_idx, "
									 "a_item_idx0, a_skill_idx0, a_skill_type0, a_use0,"
									 "a_item_idx1, a_skill_idx1, a_skill_type1, a_use1,"
									 "a_item_idx2, a_skill_idx2, a_skill_type2, a_use2,"
									 "a_item_idx3, a_skill_idx3, a_skill_type3, a_use3,"
									 "a_item_idx4, a_skill_idx4, a_skill_type4, a_use4,"
									 "a_item_idx5, a_skill_idx5, a_skill_type5, a_use5)"
									 "VALUES (%d,%d,"
									 "%d,%d,%d,%d,"
									 "%d,%d,%d,%d,"
									 "%d,%d,%d,%d,"
									 "%d,%d,%d,%d,"
									 "%d,%d,%d,%d,"
									 "%d,%d,%d,%d)")	% pChar->m_index % apet->m_index
								 % apet->m_tAIData[0].nItemNum % apet->m_tAIData[0].nSkillIdx % apet->m_tAIData[0].nSkillType % (int)apet->m_tAIData[0].cUse
								 % apet->m_tAIData[1].nItemNum % apet->m_tAIData[1].nSkillIdx % apet->m_tAIData[1].nSkillType % (int)apet->m_tAIData[1].cUse
								 % apet->m_tAIData[2].nItemNum % apet->m_tAIData[2].nSkillIdx % apet->m_tAIData[2].nSkillType % (int)apet->m_tAIData[2].cUse
								 % apet->m_tAIData[3].nItemNum % apet->m_tAIData[3].nSkillIdx % apet->m_tAIData[3].nSkillType % (int)apet->m_tAIData[3].cUse
								 % apet->m_tAIData[4].nItemNum % apet->m_tAIData[4].nSkillIdx % apet->m_tAIData[4].nSkillType % (int)apet->m_tAIData[4].cUse
								 % apet->m_tAIData[5].nItemNum % apet->m_tAIData[5].nSkillIdx % apet->m_tAIData[5].nSkillType % (int)apet->m_tAIData[5].cUse
								);

		vec.push_back(insertQuery);
	}

}

void DBManager::_SaveCharacterAttackPetTempDeleteQuery(std::vector<std::string> & vec, CPC* pChar)
{
	vec;
	pChar;
	// 임시저장 애완동물 삭제 : a_enable == 2
	std::string deletQuery = boost::str(boost::format("DELETE FROM t_apets WHERE a_owner=%d AND a_enable=2") % pChar->m_index);
	vec.push_back(deletQuery);
}

void DBManager::_SaveCharacterFactoryQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format("DELETE FROM t_characters_factory WHERE a_char_idx = %d") % pChar->m_index);
	vec.push_back(deleteQuery);

	if (pChar->m_listFactory.GetCount() <= 0)
		return;

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format("INSERT INTO t_characters_factory (a_char_idx, a_factory_idx) VALUES (%d,'") % pChar->m_index);

	CFactoryList::set_t& list = pChar->m_listFactory.getList();
	CFactoryList::set_t::iterator it = list.begin();
	CFactoryList::set_t::iterator endit = list.end();
	for(; it != endit; ++it)
	{
		CFactoryProto* p = *(it);
		insertQuery += boost::str(boost::format(" %d") % p->GetIndex());
	}
	insertQuery += "')";

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterMessengerOPTQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format("DELETE FROM t_messenger_opt WHERE a_char_idx = %d") % pChar->m_index);
	vec.push_back(deleteQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);

	CLCString gIndexList(255+1);
	CLCString gNameList(255+1);

	if(pChar->m_Friend)
	{
		pChar->m_Friend->GetGroupIndexString(gIndexList, gNameList);
		insertQuery = boost::str(boost::format("INSERT INTO t_messenger_opt VALUES ( %d, ' %s', ' %s', %d ) ")
								 % pChar->m_index % (const char*) gIndexList % (const char*) gNameList % pChar->m_Friend->GetChatColor());
	}
	else
	{
		insertQuery = boost::str(boost::format("INSERT INTO t_messenger_opt values ( %d, '', '', 0 )  ") % pChar->m_index);
	}

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterMessengerFriendQuery(std::vector<std::string> & vec, CPC* pChar, int table_no)
{
	CFriendMember* fMember = NULL;
	bool bsucc = true;
	for(int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if( !pChar->m_Friend )
			continue;

		fMember = pChar->m_Friend->GetFriendMember(i);

		if( !fMember )
			continue;

		std::string updateQuery;
		updateQuery.reserve(MAX_DB_QUERY);
		updateQuery = boost::str(boost::format("UPDATE t_friend0%d SET a_group_index = %d WHERE a_char_index = %d and a_friend_index = %d ")
								 % table_no % fMember->GetGroup() % pChar->m_index % fMember->GetChaIndex());

		vec.push_back(updateQuery);
	}
}

void DBManager::_SaveCharacterMessengerBlockFriendQuery(std::vector<std::string> & vec, CPC* pChar)
{
	CLCString blockIndexList(255+1);
	CLCString blockNameList(255+1);
	pChar->GetBlockListString(blockIndexList, blockNameList);

	std::string deleteQuery = boost::str(boost::format("DELETE FROM t_block_friend WHERE a_char_idx = %d ") % pChar->m_index);
	vec.push_back(deleteQuery);

	std::string insertQuery = boost::str(boost::format("INSERT INTO t_block_friend (a_char_idx, a_block_idx_list, a_block_name_list) VALUES (%d, '%s', '%s') " )
										 % pChar->m_index  % (const char*)blockIndexList % (const char*) blockNameList);

	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterRoyalRumbleQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format("DELETE FROM t_warground WHERE a_char_index = %d") % pChar->m_index);
	vec.push_back(deleteQuery);

	std::string insertQuery;
	insertQuery.reserve(MAX_DB_QUERY);
	insertQuery = boost::str(boost::format(
								 "INSERT INTO t_warground (a_char_index, a_total_kill_count, a_total_death_count, a_warground_point, a_warground_acc_point) VALUES (%d, %d, %d, %d, %d)")
							 % pChar->m_index % pChar->GetTotalKillCount() % pChar->GetTotalDeathCount() % pChar->GetWarGroundPoint() % pChar->GetWarGroundAccPoint());
	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterEventGomdoriQuery(std::vector<std::string> & vec, CPC* pChar)
{
	if( gserver->isActiveEvent(A_EVENT_BEAR_DOLL) )
	{
		// insert 체크용으로 사용함

		std::string selectQuery = boost::str(boost::format("SELECT a_char_index FROM t_event_gomdori_2007 WHERE a_char_index=%d LIMIT 1") % pChar->m_index);
		vec.push_back(selectQuery);

		std::string insertQuery = boost::str(boost::format("INSERT INTO t_event_gomdori_2007 (a_char_index) VALUES (%d)") % pChar->m_index);
		vec.push_back(insertQuery);

		std::string updateQuery;
		if (pChar->m_nEventGomdori2007Total > 0)
		{
			updateQuery = boost::str(boost::format("UPDATE t_event_gomdori_2007 SET a_total=%d WHERE a_char_index=%d") % pChar->m_nEventGomdori2007Total % pChar->m_index);
		}
		if (pChar->m_nEventGomdori2007FirstLose == 0)
		{
			updateQuery = boost::str(boost::format("UPDATE t_event_gomdori_2007 SET a_first_lose=UNIX_TIMESTAMP(NOW()) WHERE a_char_index=%d AND a_first_lose=0") % pChar->m_index);
		}
		if (pChar->m_nEventGomdori2007FirstWin == 0)
		{
			updateQuery = boost::str(boost::format("UPDATE t_event_gomdori_2007 SET a_first_win=UNIX_TIMESTAMP(NOW()) WHERE a_char_index=%d AND a_first_win=0") % pChar->m_index);
		}

		if (!updateQuery.empty())
			vec.push_back(updateQuery);
	}
}

void DBManager::_SaveTimerItemQuery(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_TimerItem.getDataToQuery(vec);
}

void DBManager::_SaveCharacterPetStashDeleteQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string deleteQuery = boost::str(boost::format("DELETE from t_pet_stash where a_char_idx = %d") % pChar->m_index);

	vec.push_back(deleteQuery);

	if(pChar->m_petStashManager.getCount() > 0)
	{
		pChar->m_petStashManager.GetDataToQuery(vec);
	}
}

void DBManager::_SaveCharacterPetStashQuery(std::vector<std::string> & vec, CPC* pChar)
{
	std::string selectQuery = boost::str(boost::format("SELECT * from t_pet_stash_info where a_char_index = %d") % pChar->m_index);
	vec.push_back(selectQuery);

	std::string updateQuery = boost::str(boost::format("UPDATE t_pet_stash_info set a_effect = %d where a_char_index = %d") % pChar->m_petStashManager.getEffect() % pChar->m_index);
	vec.push_back(updateQuery);

	std::string insertQuery = boost::str(boost::format("INSERT INTO t_pet_stash_info values (%d, %d)") % pChar->m_index % pChar->m_petStashManager.getEffect());
	vec.push_back(insertQuery);
}

void DBManager::_SaveCharacterSkillCoolTimeQuery(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_activeSkillList.SaveSkillCoolTime(pChar, vec);
}

void DBManager::_SaveCharacterRVRInfo(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_syndicateManager.save(vec);
}

void DBManager::_SaveCharacterGPSInfo(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_gpsManager.save(vec);
}

void DBManager::_SaveCharacterItemCollectionInfo(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_itemCollectionManager.save(vec);
}

#ifdef PREMIUM_CHAR
void DBManager::_SaveCharacterPremiumCharInfo(std::vector<std::string> & vec, CPC* pChar)
{
	pChar->m_premiumChar.save(vec);
}
#endif

void DBManager::SaveCharacterInfo(CDescriptor* d, bool disconnect)
{
	save_char_queryt_t* pushdata = new save_char_queryt_t;
	for (int index = 0; index < CHAR_SAVE_TYPE_TCHAR_MAX; ++index)
	{
		pushdata->push_back(std::vector<std::string>());
	}

	CPC* pChar = d->m_pChar;
	int table_no = pChar->m_index % 10;

	{
		// Character Query
		_SaveCharaterQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR], pChar, table_no);
	}

	{
		// Guild Point Query
		_SaveGuildPointQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_GUILD_POINT], pChar);
	}

	{
		// Quest Query
		_SaveQuestQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_QUEST], pChar, table_no);
	}

	{
		// Affinity Query
		_SaveAffinityQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_AFFINITY], pChar);
	}

	{
		// Assit Query
		_SaveAssistQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_ASSIST], pChar);
	}

	{
		// Assist ABS Time Query
		_SaveAssistABSTimeQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_ASSIST_ABS_TIME], pChar);
	}

	{
		// Inven Query
		_SaveCharInvenQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_INVEN], pChar,table_no);
	}

	{
		// Stash Query
		_SaveCharcterStashQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_STASH], pChar);
	}

	{
		// Quick Slot Query
		_SaveCharacterQuickSlotQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_QUICK_SLOT], pChar, table_no);
	}

	{
		// Mempos Query
		_SaveCharacterMemposQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_MEMPOS], pChar);
	}

	{
		// Cash Item Date Query
		_SaveCharacterCashItemDateQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_CASH_ITEM_DATE], pChar, d->m_index);
	}

	{
		// personal dungeon Query
		_SaveCharacterPersonalDungeonQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PD_DATA], pChar, table_no);
	}

	{
		// Pet Query
		CPet* pet = pChar->m_petList;

		while (pet)
		{
			{
				//Pet Pre Query :: 애완 동물 상태를 2로 변경
				_SaveCharacterPetPreQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET_PRE], pChar);
			}

			{
				// Pet Query
				_SaveCharacterPetQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET], pChar, pet);
			}
			{
				// Pet Name Query
				_SaveCharacterPetNameQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET_NAME], pChar, pet);
			}

			pet = pet->m_nextPet;
		}

		{
			// Pet Temp Delete Query
			_SaveCharacterPetTempDeleteQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET_TEMP_DELETE], pChar);
		}
	}

	{
		// Attack Pet Query
		{
			// Attack Pet Pre Query :: 애완 동물 상태를 2로 변경
			_SaveCharacterAttackPetPreQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_APET_PRE], pChar);
		}

		CAPet* apet = pChar->m_pApetlist;
		while (apet)
		{
			{
				// Attack Pet Query
				_SaveCharacterAttackPetQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_APET], pChar, apet);
			}

			{
				// Attack Pet Inven Query
				_SaveCharacterAttackPetInvenQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_APET_INVEN], pChar, apet);
			}

			{
				// Attack Pet AI Query
				_SaveCharacterAttackPetAIQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_APET_AI], pChar, apet);
			}

			apet = apet->m_pNextPet;
		}

		{
			// Attack Pet Temp Delete Query
			_SaveCharacterAttackPetTempDeleteQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_APET_TEMP_DELETE], pChar);
		}

	}

	{
		// Factory Query
		_SaveCharacterFactoryQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_CHARACTER_FACTORY], pChar);
	}

	{
		// Messenger OPT Query
		_SaveCharacterMessengerOPTQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_MESSENGER_OPT], pChar);
	}

	{
		// Messenger Friend Query
		_SaveCharacterMessengerFriendQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_FRIEND], pChar, table_no);
	}

	{
		// Messenger Block Friend Query
		_SaveCharacterMessengerBlockFriendQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_BLOCK_FRIEND], pChar);
	}


	{
		// War Ground ( Royal Rumble )
		_SaveCharacterRoyalRumbleQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_WAR_GROUND], pChar);
	}


	{
		// Event Gomdori Query
		_SaveCharacterEventGomdoriQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_EVENT_GOMDORI], pChar);
	}

	{
		// Timet Item Query
		_SaveTimerItemQuery((*pushdata)[CHAR_SAVE_TYPE_TIMER_ITEM], pChar);
	}

	{
		// Pet Stash Delete Query
		_SaveCharacterPetStashDeleteQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET_STASH], pChar);
	}

	{
		// Pet Stash Info Query
		_SaveCharacterPetStashQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_PET_STASH_INFO], pChar);
	}

	{
		std::vector<std::string>& vec = (*pushdata)[CHAR_SAVE_TYPE_TCHAR_WEAR_ITEM];
		pChar->m_wearInventory.SaveWearInvenInfo(vec);
	}

	// Skill Cool Time Query
	{
		_SaveCharacterSkillCoolTimeQuery((*pushdata)[CHAR_SAVE_TYPE_TCHAR_SKILL_COOLTIME], pChar);
	}

	// RVR info Query
	{
		_SaveCharacterRVRInfo((*pushdata)[CHAR_SAVE_TYPE_RVR_INFO], pChar);
	}

	// GPS info query
	{
		_SaveCharacterGPSInfo((*pushdata)[CHAR_SAVE_TYPE_GPS_INFO], pChar);
	}

	// Item Collection Query
	{
		_SaveCharacterItemCollectionInfo((*pushdata)[CHAR_SAVE_TYPE_ITEM_COLLECTION_INFO], pChar);
	}

#ifdef PREMIUM_CHAR
	// premium char
	{
		_SaveCharacterPremiumCharInfo((*pushdata)[CHAR_SAVE_TYPE_PREMIUM_CHAR_INFO], pChar);
	}
#endif

	DBManager::instance()->PushSaveCharacter(d, pushdata, disconnect);

	GAMELOG << init("CHAR_UPDATE", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname);
	GAMELOG << d->m_pChar->m_inventory.getMoney();
	GAMELOG << end;
}

