#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "DratanCastle.h"
#include "Battle.h"
#include "../ShareLib/packetType/ptype_old_do_action.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"


//////////////////////////////////////////////////////////////////////////
void do_SkillLearn(CPC* ch, CNetMsg::SP& msg);
void do_SkillReady(CPC* ch, CNetMsg::SP& msg);
void do_SkillCancel(CPC* ch, CNetMsg::SP& msg);
void do_SkillFire(CPC* ch, CNetMsg::SP& msg);
#ifdef HANARO_EVENT
void do_SkillHanaroEvent(CPC * ch, CNetMsg::SP& msg);
#endif
void do_SealSkillLearn(CPC* ch, CNetMsg::SP& msg);
void do_SkillMidLevelSupport(CPC* ch, CNetMsg::SP& msg);
void do_Skill_UseEP ( CPC* ch, CNetMsg::SP& msg );
//////////////////////////////////////////////////////////////////////////


void do_Skill(CPC* ch, CNetMsg::SP& msg)
{

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckRespond(ch);

	if (DEAD(ch) || ch->IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return ;

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_SKILL_LEARN:
		do_SkillLearn(ch, msg);
		break;

	case MSG_SKILL_READY:
		do_SkillReady(ch, msg);
		break;

	case MSG_SKILL_FIRE:
		do_SkillFire(ch, msg);
		break;

	case MSG_SKILL_CANCEL:
		do_SkillCancel(ch, msg);
		break;

#ifdef HANARO_EVENT
	case MSG_SKILL_HANARO_EVENT:
		do_SkillHanaroEvent(ch, msg);
		break;
#endif

	case MSG_SKILL_LEARN_SEAL:
		do_SealSkillLearn(ch, msg);
		break;

	case MSG_SKILL_MIDLEVEL_SUPPORT:
		do_SkillMidLevelSupport(ch, msg);
		break;

	case MSG_SKILL_USEEP:
		do_Skill_UseEP( ch, msg );
		break;

	default:
		{
			LOG_ERROR("HACKING : invalid subtype[%d]. charIndex[%d]", pBase->subType, ch->m_index);
			ch->m_desc->Close("invalid subtype");
			return;
		}
		break;
	}
}

void do_SkillLearn(CPC* ch, CNetMsg::SP& msg)
{
	try
	{
		RequestClient::skillLearn* packet = reinterpret_cast<RequestClient::skillLearn*>(msg->m_buf);

		if (packet->npcIndex <= 0) // use item
		{
			if (ch->m_inventory.FindByDBIndex(2868) == NULL)
			{
				LOG_ERROR("HACKING : not found item(2868). charIndex[%d]", ch->m_index);
				ch->m_desc->Close("not found item");
				return;
			}
		}
		else
		{
			CNPC* pNpc = (CNPC*)ch->m_pArea->FindCharInCell(ch, packet->npcIndex, MSG_CHAR_NPC, true);
			if (pNpc == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_ERROR_NOT_EXIST_NPC, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}

		const CSkillProto* proto = gserver->m_skillProtoList.Find(packet->skillIndex);
		if (proto == NULL)
		{
			LOG_ERROR("HACKING : not found skill[%d]. charIndex[%d]", packet->skillIndex, ch->m_index);
			ch->m_desc->Close("not found skill");
			return;
		}

		CSkillList* list = NULL;
		if (proto->IsActive())
			list = &ch->m_activeSkillList;
		else if (proto->IsPassive())
			list = &ch->m_passiveSkillList;
		else
			list = &ch->m_etcSkillList;

		if (list == NULL)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		CSkill* skill = list->Find(packet->skillIndex);
		int nextLevel = (skill) ? skill->m_level + 1 : 1;

		const CSkillLevelProto* levelproto = proto->Level(nextLevel);
		if (!levelproto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		// 직업 검사 : TODO : 전직 이후 처리 : 함수로 만들기
		if (!proto->IsSameJob(ch))
			return ;

		// 레벨 검사
		if (levelproto->m_learnLevel > ch->m_level)
			throw MSG_SKILL_LEARN_ERROR_LEVEL;

		// SP 검사
		if (levelproto->m_learnSP * 10000 > ch->m_skillPoint)
			throw MSG_SKILL_LEARN_ERROR_SP;

		// 아이템 검사
		LONGLONG needMoney = 0;

		int i;
		item_search_t vec[MAX_SKILL_LEARN_ITEM];
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if (levelproto->m_learnItemIndex[i] < 0)
				continue ;

			if(levelproto && levelproto->m_learnItemIndex[i] == NAS_ITEM_DB_INDEX)
			{
				needMoney += levelproto->m_learnItemCount[i];

				if(needMoney > ch->m_inventory.getMoney())
					throw MSG_SKILL_LEARN_ERROR_ITEM;

				continue;
			}

			int sc = ch->m_inventory.searchItemByCondition(levelproto->m_learnItemIndex[i], 0, 0, vec[i]);
			if (sc == 0 || sc < levelproto->m_learnItemCount[i])
			{
				throw MSG_SKILL_LEARN_ERROR_ITEM;
			}
		}

		// 스킬 검사
		for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
		{
			if (levelproto->m_learnSkillIndex[i] < 0)
				continue ;
			//스폐셜 리스트의 전직시 연계 스킬 조건을 무시하게 되는 스킬리스트
			if (ch->m_job == JOB_SORCERER && ch->m_job2 == JOB_2ND_SPECIALLIST)
			{
				if(levelproto->m_learnSkillIndex[i] == 293 /*땅의 정령 소환*/
						|| levelproto->m_learnSkillIndex[i] == 294 /*불의 정령 소환*/
						|| levelproto->m_learnSkillIndex[i] == 300 /*물의 정령 소환*/
						|| levelproto->m_learnSkillIndex[i] == 301 /*바람의 정령 소환*/)
				{
					continue;
				}
			}
			const CSkillProto* lproto = gserver->m_skillProtoList.Find(levelproto->m_learnSkillIndex[i]);
			CSkill* lskill;
			if (lproto->IsActive())
				lskill = ch->m_activeSkillList.Find(levelproto->m_learnSkillIndex[i]);
			else if (lproto->IsPassive())
				lskill = ch->m_passiveSkillList.Find(levelproto->m_learnSkillIndex[i]);
			else
				lskill = ch->m_etcSkillList.Find(levelproto->m_learnSkillIndex[i]);
			if (lskill == NULL || lskill->m_level < levelproto->m_learnSkillLevel[i])
				throw MSG_SKILL_LEARN_ERROR_SKILL;
		}

// 050401 : bs : 추가 조건 : 스탯 4종
		if (ch->m_statpt_str < levelproto->m_learnstr)
			throw MSG_SKILL_LEARN_ERROR_STR;
		if (ch->m_statpt_dex < levelproto->m_learndex)
			throw MSG_SKILL_LEARN_ERROR_DEX;
		if (ch->m_statpt_int < levelproto->m_learnint)
			throw MSG_SKILL_LEARN_ERROR_INT;
		if (ch->m_statpt_con < levelproto->m_learncon)
			throw MSG_SKILL_LEARN_ERROR_CON;
// --- 050401 : bs : 추가 조건 : 스탯 4종

		// SP 소모
		ch->m_skillPoint -= levelproto->m_learnSP * 10000;

		// 아이템 및 돈(Nas) 소모
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if (levelproto->m_learnItemIndex[i] < 0)
				continue ;

			ch->m_inventory.deleteItem(vec[i], levelproto->m_learnItemCount[i]);
		}

		if (needMoney > 0)
		{
			ch->m_inventory.decreaseMoney(needMoney);
		}

		// 스킬 추가
		if (skill == NULL)
		{
			// 새로 배움
			skill = gserver->m_skillProtoList.Create(packet->skillIndex);
			if (!skill)
				throw MSG_SKILL_LEARN_ERROR_SYSTEM;
			if (proto->IsActive())
				ch->m_activeSkillList.Add(skill);
			else if (proto->IsPassive())
				ch->m_passiveSkillList.Add(skill);
			else
				ch->m_etcSkillList.Add(skill);
		}
		else
		{
			// 있는 스킬 레벨 업
			if (skill->m_level >= proto->m_maxLevel)
				return ;
			skill->m_level++;
		}

		// 소환수 스킬은 소환수에게도 추가 : 액티브만
		if ((proto->m_type == ST_MELEE || proto->m_type == ST_RANGE || proto->m_type == ST_MAGIC) && proto->CheckSorcererFlag(SSF_USE_ALL))
		{
			int needFlag = 0;
			CElemental* p = ch->m_elementalList;
			while (p)
			{
				switch (p->GetElementalType())
				{
				case ELEMENTAL_FIRE:
					needFlag = SSF_USE_FIRE;
					break;
				case ELEMENTAL_WIND:
					needFlag = SSF_USE_WIND;
					break;
				case ELEMENTAL_EARTH:
					needFlag = SSF_USE_EARTH;
					break;
				case ELEMENTAL_WATER:
					needFlag = SSF_USE_WATER;
					break;
				default:
					needFlag = -1;
					break;
				}
				if (needFlag != -1)
				{
					if (proto->CheckSorcererFlag(needFlag))
						p->AddSkill(packet->skillIndex, skill->m_level);
				}
				p = p->m_nextElemental;
			}
		}
		else
		{
			CElemental* p = ch->m_elementalList;
			while (p)
			{
				p->CalcStatus(true);
				p = p->m_nextElemental;
			}
		}

		GAMELOG << init("SKILL LEARN", ch)
				<< skill->m_proto->m_index
				<< delim
				<< skill->m_level
				<< end;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillLearnMsg(rmsg, skill, ch);
			SEND_Q(rmsg, ch->m_desc);
		}

		if (levelproto)
		{
			for (i = 0; i < MAX_SKILL_MAGIC; i++)
			{
				if (levelproto->m_magic[i])
				{
					switch (levelproto->m_magic[i]->m_type)
					{
					case MT_IMMUNE:
						switch (levelproto->m_magic[i]->m_subtype)
						{
						case MST_IMMUNE_BLIND:
							ch->m_assist.CureAssist(MST_ASSIST_BLIND, 99);
							break;
						}
						break;
					}
				}
			}
		}

		ch->CalcStatus(true);
	}
	catch (MSG_SKILL_LEARN_ERROR_TYPE learnerr)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillLearnErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_SkillReady(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::skillReady* packet = reinterpret_cast<RequestClient::skillReady*>(msg->m_buf);

	if (msg->getSize() != sizeof(RequestClient::skillReady))
	{
		LOG_ERROR("HACKING : invalid packet size[%d] / serverSize[%d]. charIndex[%d]", msg->getSize(), sizeof(RequestClient::skillReady), ch->m_index);
		ch->m_desc->Close("invalid packet size");
		return;
	}

	CSkill* skill = NULL;
	const CSkillProto* skillproto;
	bool bIsGuildSkill = false;
	const CSkillLevelProto* levelproto;
	CCharacter* tch = NULL;
	int i;
	int nFailExtraInfo = 0;
	bool bCheck = true;

	int nNeedMP = 0;
	int nNeedGP = 0;

	CNPC* pNPC = NULL;
	CElemental* pElemental = NULL;
	CPet* pPet = NULL;

	CCharacter* chMP;
	CCharacter* charSkill = NULL;

	switch (packet->charType)
	{
	case MSG_CHAR_NPC:
		if (ch->m_pZone == NULL || !ch->m_pZone->IsPersonalDungeon())
			return ;
		pNPC = TO_NPC(ch->m_pArea->FindCharInCell(ch, packet->charIndex, (MSG_CHAR_TYPE)packet->charType));
		charSkill = pNPC;
		break;

	case MSG_CHAR_PET:
		if (ch->m_bImmortal || ch->IsDisable())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, nFailExtraInfo);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pPet = ch->GetPet();
		if (!pPet || !pPet->IsMount() || pPet->m_index != packet->charIndex)
			return ;

		//죽은 펫으로 스킬을 사용하려고 하면
		if(pPet->m_hp <= 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, nFailExtraInfo);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 펫의 스킬을 PC가 사용
		charSkill = pPet;
		break;

	case MSG_CHAR_PC:
		if ( (ch->m_bImmortal && ch->m_bFirstDoIt == false) || ch->IsDisable() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (ch->m_index != packet->charIndex || ch->IsDisable())
			return ;
		charSkill = ch;
		break;

	case MSG_CHAR_ELEMENTAL:
		if (ch->m_bImmortal || ch->IsDisable())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, nFailExtraInfo);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pElemental = ch->GetElemental(packet->charIndex);
		if (pElemental)
		{
			if (pElemental->GetOwner() != ch)
				return ;
		}
		charSkill = pElemental;
		break;

	case MSG_CHAR_APET:
		{
			CAPet* apet = ch->GetAPet();
			if( apet && apet->IsSummon() )
				charSkill = apet;
			else
				return;
		}
		break;

	default:
		break;
	}

	if (charSkill == NULL)
		return ;

	try
	{
// 공성중에 텔레키네시스 스킬 사용못하게 막음
#ifndef	BUGFIX_USE_SKILL_TO_NPC
#ifdef TIME_IN_WARCASTLE_BUG_FIX
		if(packet->skillIndex == 169)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
			if (castle)
			{
				if(packet->charType == MSG_CHAR_PC)
				{
					// 공성중이거나 시작 1분 전에는 공성지역으로 이동 불가
					if (castle->GetState() != WCSF_NORMAL || castle->IsWarCastleReady())
					{
						if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) & MATT_WAR ||
								ch->m_pZone->IsWarZone((int)GET_X(ch), (int)GET_Z(ch)) == true)
						{
							// 스킬 사용 못하는 에러메세지 발송
							throw MSG_SKILL_ERROR_CANNOT_SPELL;
						}
					}
				}
			}
		}
#endif
#endif

		if(packet->skillIndex == SKILL_GUILD_PARTY_EXP && ch->m_party == NULL)
		{
			throw MSG_SKILL_ERROR_CANNOT_SPELL;
		}

		//길드룸에서 서먼타입 스킬 방지
		if(ch->m_pZone->m_index == ZONE_GUILDROOM)
		{
			const CSkillProto* proto = gserver->m_skillProtoList.Find(packet->skillIndex);
			if(proto == NULL)
			{
				throw MSG_SKILL_ERROR_NOTFOUND;
			}
			if(proto->IsSummon())
			{
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			}
		}

		if(ch->m_pZone->IsPersonalDungeon())
		{
			const CSkillProto* proto = gserver->m_skillProtoList.Find(packet->skillIndex);
			if(proto == NULL)
			{
				throw MSG_SKILL_ERROR_NOTFOUND;
			}
			if(proto->m_flag & SF_NOT_SINGLE)
			{
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			}
		}
		// 마운트 가능 지역에서만 비행 가능
		if( packet->skillIndex == FLY_SKILL && ch->m_pZone->m_bCanMountPet == false )
		{
			throw MSG_SKILL_ERROR_CANNOT_SPELL;
		}

		// 스킬 찾기
		skillproto = gserver->m_skillProtoList.Find(packet->skillIndex);

		if(skillproto == NULL)
			throw MSG_SKILL_ERROR_NOTFOUND;

		if(skillproto->m_flag & SF_GUILD)
		{
			if(ch->m_guildInfo == NULL || ch->m_guildInfo->guild() == NULL)
			{
				LOG_INFO("HACKING? : invalid skill[%d]. char_index[%d]", skillproto->m_index, ch->m_index);
				ch->m_desc->Close("invalid GuildSkill");
				return ;
			}
			if(skillproto->m_targetType != STT_GUILD_MEMBER_SELF &&
				ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
			{
				LOG_INFO("HACKING? : invalid skill[%d]. char_index[%d]", skillproto->m_index, ch->m_index);
				ch->m_desc->Close("invalid GuildSkill");
				return ;
			}
			bIsGuildSkill = true;
			skill = ch->m_guildInfo->guild()->FindSkill(packet->skillIndex);
		}
		else
			skill = charSkill->FindSkill(packet->skillIndex);

		if( packet->charType == MSG_CHAR_APET )
		{
			CAPet* apet = TO_APET(charSkill);

			if( skill != NULL && skill->m_proto && skill->m_proto->IsPassive() )
				throw MSG_SKILL_ERROR_NOTFOUND;

			if( skill == NULL && apet && apet->m_pProto->m_nSummonSkill[(unsigned int)apet->m_cTransSate] == packet->skillIndex )
			{
				skill = gserver->m_skillProtoList.Create(packet->skillIndex);
				if (skill == NULL || skill->m_proto == NULL)
					throw MSG_SKILL_ERROR_NOTFOUND;
			}
		}

		if (skill == NULL || skill->m_proto == NULL)
			throw MSG_SKILL_ERROR_NOTFOUND;

		if(skill->m_enable == 0)
		{
			LOG_INFO("HACKING? : invalid skill[%d]. char_index[%d]", skillproto->m_index, ch->m_index);
			ch->m_desc->Close("invalid Skill");
			return ;
		}

		if( (skill->m_proto->m_flag & SF_TOGGLE) && skill->m_toggle == true )
		{
			bCheck = false;
		}

		levelproto = skill->m_proto->Level(skill->m_level);
		if (levelproto == NULL)
			throw MSG_SKILL_ERROR_NOTFOUND;

		if (IS_PC(charSkill) && charSkill == ch)
		{
			// 인비저빌리티
			if (skill->m_proto->m_index == 131)
			{
				// 암제에선 인비저빌리티 사용 못한다.
				if (ch->m_pZone && ch->m_pZone->m_index == ZONE_ALTER_OF_DARK)
					throw MSG_SKILL_ERROR_CANNOT_SPELL;
			}
		}

		if( skillproto->m_type == ST_SUMMON_SKILL )
		{
			if( skillproto->m_levelproto[0]->m_magic[0]->m_type == MT_ASSIST )
			{
				switch( skillproto->m_levelproto[0]->m_magic[0]->m_subtype)
				{
				case MST_ASSIST_SOUL_TOTEM_BUFF:
				case MST_ASSIST_SOUL_TOTEM_ATTK:
					if( ch->GetSummonNpc(SUMMON_NPC_TYPE_TOTEM) )
						throw MSG_SKILL_ERROR_CANNOT_SPELL;

					else if( ch->m_pZone->IsPersonalDungeon() )
						throw MSG_SKILL_ERROR_INVAL_STATE_PSERSONAL_DUNGEON;

					break;
				case MST_ASSIST_TRAP:
				case MST_ASSIST_PARASITE:
					if( ch->IsInPeaceZone(true) )
						throw MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE;

					else if( ch->GetBombSummonCont() >= 3 )
						throw MSG_SKILL_ERROR_CANNOT_SPELL;

					else if( ch->m_pZone->IsPersonalDungeon() )
						throw MSG_SKILL_ERROR_INVAL_STATE_PSERSONAL_DUNGEON;

					break;
				case MST_ASSIST_TOTEM_ITEM_BUFF:
				case MST_ASSIST_TOTEM_ITEM_ATTK:
					if( ch->GetSummonNpc(SUMMON_NPC_TYPE_TOTEM_ITEM) )
						throw MSG_SKILL_ERROR_CANNOT_SPELL;

					else if( ch->m_pZone->IsPersonalDungeon() )
						throw MSG_SKILL_ERROR_INVAL_STATE_PSERSONAL_DUNGEON;

					break;
				}
			}
		}

#ifdef RESTRICT_PVP_SKILL
		if (IS_PC(charSkill) && charSkill == ch)
		{
			// 인비저빌리티
			if (skill->m_proto->m_index == 131)
			{
				if (ch->m_nRestrictPvPSkillDelaySec > 0)
					throw MSG_SKILL_ERROR_CANNOT_INVISIBLE;
			}
			// 데스 모션
			else if (skill->m_proto->m_index == 191)
			{
				if (!IS_IN_CELL(ch))
					throw MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC;

				int sx = ch->m_cellX - 1;
				int ex = ch->m_cellX + 1;
				int sz = ch->m_cellZ - 1;
				int ez = ch->m_cellZ + 1;

				int x, z;

				for (x = sx; x <= ex; x++)
				{
					if (x < 0 || x >= ch->m_pArea->m_size[0]) continue;
					for (z = sz; z <= ez; z++)
					{
						if (z < 0 || z >= ch->m_pArea->m_size[1]) continue;
						CCharacter *pChar;
						for (pChar = ch->m_pArea->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
						{
							if (IS_PC(pChar) && pChar != ch)
							{
								if (GetDistance(pChar, ch) > 15.0f)
									continue ;

								CPC* pTestPC = TO_PC(pChar);
								if (pTestPC->m_admin > 1)
									continue ;
								if (pTestPC->m_party != NULL && ch->m_party == pTestPC->m_party)
									continue ;
								if (pTestPC->m_guildInfo && pTestPC->m_guildInfo->guild() && ch->m_guildInfo && pTestPC->m_guildInfo->guild() == ch->m_guildInfo->guild())
									continue ;
								throw MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC;
							}
						}
					}
				}
			}
		}
#endif // RESTRICT_PVP_SKILL

		if(ch->m_pZone->isRVRZone() && skill->m_proto->isRVRNotUse())
		{
			// rvr 존에서는 사용할 수 없는 스킬
			throw MSG_SKILL_ERROR_CANNOT_ZONE;
		}

		if ( IS_PC(charSkill) && bCheck == true)
		{
			// 존검사 추가 //  [2/20/2009 KwonYongDae]
#if defined(LC_BILA)
			if( charSkill->m_pZone && charSkill->m_pZone->IsOXQuizRoom() )
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
#endif // LC_BRZ

			{
				CPC* pc = TO_PC(charSkill);
				if(pc->m_assist.m_state & AST_SAFEGUARD && pc->m_bFirstDoIt == false && (skill->m_proto->m_flag & SF_HELP) == false)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_DO_NOT_ATTACK_DO_MOVE);
					SEND_Q(rmsg, pc->m_desc);
					throw MSG_SKILL_ERROR_CANNOT_SPELL;
				}
			}

#ifdef EXTREME_CUBE_VER2
			// 스킬 허용 존 검사
			if(skill->m_proto->IsActive())
			{
				if(!skill->m_proto->CheckAllowZone((char)(charSkill->m_pZone->m_index)))
					throw MSG_SKILL_ERROR_CANNOT_SPELL;
			}
#endif // EXTREME_CUBE_VER2

			// 스탯 조건 검사
			const CSkillLevelProto* level1 = skill->m_proto->Level(1);
			if (!level1)
				throw MSG_SKILL_ERROR_NOTFOUND;
			if (ch->m_statpt_str < level1->m_learnstr)
				throw MSG_SKILL_ERROR_STATPOINT_STR;
			if (ch->m_statpt_dex < level1->m_learndex)
				throw MSG_SKILL_ERROR_STATPOINT_DEX;
			if (ch->m_statpt_int < level1->m_learnint)
				throw MSG_SKILL_ERROR_STATPOINT_INT;
			if (ch->m_statpt_con < level1->m_learncon)
				throw MSG_SKILL_ERROR_STATPOINT_CON;

			// 퍼펙트 바디
			if (packet->skillIndex == 152 && ch->m_hp > ch->m_maxHP * 0.7f)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			// 프렌지
			if (packet->skillIndex == 153 && ch->m_hp > ch->m_maxHP * 0.7f)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			// 버서크
			if (packet->skillIndex == 228 && ch->m_hp > ch->m_maxHP * 0.7f)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;

			// 소환, 강신, 테이밍 검사
			if (levelproto->m_magic[0] && levelproto->m_magic[0]->m_levelproto[0])
			{
				if (levelproto->m_magic[0]->m_type == MT_OTHER)
				{
					if (levelproto->m_magic[0]->m_subtype == MST_OTHER_SUMMON)
					{
						// 스페셜리스트는 소환 불능
						if (ch->m_job == JOB_SORCERER && ch->m_job2 == JOB_2ND_SPECIALLIST)
							throw MSG_SKILL_ERROR_NOTFOUND;

						// 전직 이후에는 폴암 착용시에만 가능
						if (ch->m_job == JOB_SORCERER && ch->m_job2 == JOB_2ND_ELEMENTALLIST)
						{
							if (!ch->m_wearInventory.wearItemInfo[WEARING_WEAPON])
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
							if (!ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsWeaponType())
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
							if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto->getItemSubTypeIdx() != IWEAPON_POLEARM)
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
						}

						// 소환까지 남은 시간이 있으면 오류 정보에 남은 시간을 보냄
						bool bCheckType;
						char nCheckDuplication;
						if (!ch->CanSummonElemental(levelproto->m_magic[0]->m_levelproto[0]->m_nPowerValue, &nCheckDuplication, &nFailExtraInfo, &bCheckType))
						{
							if (nFailExtraInfo > 0)
								throw MSG_SKILL_ERROR_ELEMENTAL_TIME;
							if (bCheckType)
								throw MSG_SKILL_ERROR_NOTFOUND;
							throw (MSG_SKILL_ERROR_TYPE)(MSG_SKILL_ERROR_ELEMENTAL_FIRE + nCheckDuplication);
						}
					}

					else if (levelproto->m_magic[0]->m_subtype == MST_OTHER_EVOCATION)
					{
						// 마운트 후 강신 MSG가 서버로 들어왔을 때 막아버린다.
						if(ch->m_wearInventory.wearItemInfo[WEARING_PET] != NULL)
						{
							CItem* pItem = NULL;
							pItem = ch->m_wearInventory.wearItemInfo[WEARING_PET];
							if(pItem->IsPet())
							{
								CPet* pet = NULL;
								pet = ch->GetPet();
								if(pet->IsMount())
								{
									throw MSG_SKILL_ERROR_CANNOT_SPELL;
								}
							}

							if(pItem->IsAPet())
							{
								CAPet* apet = NULL;
								apet = ch->GetAPet();

								if(apet->IsMount())
								{
									throw MSG_SKILL_ERROR_CANNOT_SPELL;
								}
							}
						}
					}
				}
			}

			for (i = 0; i < MAX_SKILL_MAGIC; i++)
			{
				// 나이트 쉐도우 스킬 조건 검사
				if (levelproto->m_magic[i] && levelproto->m_magic[i]->m_levelproto[i])
				{
					if (levelproto->m_magic[i]->m_type == MT_OTHER)
					{
						if (levelproto->m_magic[i]->m_subtype == MST_OTHER_FLY)
						{
							CPC* pPC = TO_PC(charSkill);
							if ( pPC == NULL || pPC->m_wearInventory.wearItemInfo[WEARING_BACKWING] == NULL)
								throw MSG_SKILL_ERROR_CANNOT_SPELL;
						}

						// 영혼 소유 개수가 10개이면 더이상 흡수를 못한다.
						if (levelproto->m_magic[i]->m_subtype == MST_OTHER_SOUL_DRAIN)
						{
							CPC* pPC = TO_PC(charSkill);
							if (pPC == NULL || pPC->GetSoul_Count() >= MAX_SOUL_COUNT)
								throw MSG_SKILL_ERROR_SOUL_COUNT_ALREADY_FULL;
						}
					}
					// 증복 사용 금지
					if ( levelproto->m_magic[i]->m_type == MT_ASSIST
							&& levelproto->m_magic[i]->m_subtype == MST_ASSIST_MERCENARY )
					{
						CPC* pPC = TO_PC(charSkill);
						if (pPC == NULL || pPC->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == NULL)
							throw MSG_SKILL_ERROR_CANNOT_SPELL;
					}
				} // if
			} // for

#ifdef LC_GAMIGO
			if( charSkill->m_pZone && charSkill->m_pZone->IsPersonalDungeon())
			{
				for (i = 0; i < MAX_SKILL_MAGIC; i++)
				{
					if(levelproto->m_magic[i] && levelproto->m_magic[i]->m_type == MT_ASSIST)
					{
						switch(levelproto->m_magic[i]->m_subtype)
						{
						case MST_ASSIST_HOLD:
						case MST_ASSIST_STONE:
						case MST_ASSIST_STURN:
						case MST_ASSIST_POISON:
							throw MSG_SKILL_ERROR_CANNOT_SPELL;
						}
					}
				}
			}
#endif // LC_GAMIGO

			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;

				//나스 소모스킬 사용조건 검사( 나스 인벤토리 제거 패치)
				if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() < levelproto->m_needItemCount[i])
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
				else if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() >= levelproto->m_needItemCount[i])
					continue;

				item_search_t vec;
				int sc = ch->m_inventory.searchItemByCondition(levelproto->m_needItemIndex[i], 0, 0, vec);
				if (sc == 0 || sc < levelproto->m_needItemCount[i])
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
			}

			//death 플래그가 없는 스킬일 경우 hp가 0인 유저를 타겟을 잡고 사용하지 못한다.
			tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->targetIndex, (MSG_CHAR_TYPE)packet->targetType, true);
			if(tch == NULL)
				throw MSG_SKILL_ERROR_INVAL_TARGET;

			//pvp보호 아이템 체크
			if( (skill->m_proto->m_flag & SF_HELP) == false )
			{
				if (checkPvPProtect(ch, tch) == false)
				{
					throw MSG_SKILL_ERROR_NON_MSG;
				}
			}

			if( !(skill->m_proto->m_applyState & SCT_DEATH) )
			{
				if(tch->m_hp <= 0)
					throw MSG_SKILL_ERROR_INVAL_TARGET;

				if( IS_PC(tch)
						&& tch->m_pZone->IsPersonalDungeon() == false
						&& skill->m_proto->m_targetType != STT_SELF_ONE
						&& !(skill->m_proto->m_flag & SF_HELP)
						&&  tch->m_level <= 15 )
				{
					throw MSG_SKILL_ERROR_INVAL_TARGET;
				}
			}

			//토글영향 (달리는 중 사용할 시 타겟의 정보가 .....)
			if (ch->IsInPeaceZone(true) && !tch->IsInPeaceZone(false)
				&& skill->m_proto->m_index == 688)
			{
				throw MSG_SKILL_ERROR_INVAL_STATE_STAND;
			}
		} // PC의 조건 검사

		if( IS_APET(charSkill) && bCheck == true )
		{
			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;

				//나스 소모스킬 사용조건 검사( 나스 인벤토리 제거 패치)
				if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() < levelproto->m_needItemCount[i])
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
				else if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() >= levelproto->m_needItemCount[i])
					continue;

				CPC* pOwner = TO_APET(charSkill)->GetOwner();
				if( pOwner )
				{
					item_search_t vec;
					int sc = pOwner->m_inventory.searchItemByCondition(levelproto->m_needItemIndex[i], 0, 0, vec);
					if (sc == 0 || sc < levelproto->m_needItemCount[i])
						throw MSG_SKILL_ERROR_INSUFF_ITEM;
				}
			}

			// 변신중 스킬 사용 불가
			CAPet *apet = TO_APET(charSkill);
			if( apet && apet->m_cTransSate > 0 )
			{
				if( packet->skillIndex != apet->m_pProto->m_nSummonSkill[(unsigned int)apet->m_cTransSate] )		// 소환스킬이 아닐경우
					throw MSG_SKILL_ERROR_CANNOT_SPELL;
			}
		}

		// 펫 스킬은 찾고 나서 스킬 사용자를 PC로 변경
		if (IS_PET(charSkill))
			charSkill = ch;

		if (charSkill == NULL || charSkill->m_pArea == NULL)
			throw MSG_SKILL_ERROR_NOTREADY;

		// 스킬 사용 가능 상태 검사
		if (!charSkill->CanSpell())
			throw MSG_SKILL_ERROR_CANNOT_SPELL;

		// 현재 스킬 사용 중인지 검사
		if (charSkill->m_currentSkill)
		{
			if (gserver->m_pulse - charSkill->m_currentSkill->m_usetime >= charSkill->m_currentSkill->m_proto->m_readyTime + charSkill->m_currentSkill->m_proto->m_stillTime + charSkill->m_currentSkill->m_proto->m_fireTime - 15)
			{
				charSkill->m_currentSkill->Cancel(charSkill);
				charSkill->m_currentSkill = NULL;
			}
			else
				throw MSG_SKILL_ERROR_ALREADY_USE;
		}

		// 스킬 상태 보기 : 재사용 대기 중인지
		if (!skill->IsReady(charSkill))
			throw MSG_SKILL_ERROR_NOTREADY;

		// 소환수는 MP 공유
		chMP = (IS_ELEMENTAL(charSkill)) ? ch : charSkill;

		if(bCheck == true)
		{
			// 사용 조건 검사 : hp
			if ( levelproto->m_needHP != 0 && charSkill->m_hp <= levelproto->m_needHP)
				throw MSG_SKILL_ERROR_INSUFF_HP;

			// 사용 조건 검사 : mp
			nNeedMP = levelproto->m_needMP;
			if (charSkill == ch)
			{
				if (ch->m_opDecreaseSkillMP > 0)
					nNeedMP -= nNeedMP * ch->m_opDecreaseSkillMP / 100;
#ifdef ASSIST_DECREASE_SKILL_MP
				if (ch->m_assist.m_avRate.decreaseskillmp > 0)
					nNeedMP -= nNeedMP *
					ch->m_assist.m_avRate.decreaseskillmp / 100;
				if (ch->m_assist.m_avAddition.decreaseskillmp > 0)
					nNeedMP -= ch->m_assist.m_avAddition.decreaseskillmp;

				if (nNeedMP < 0)
					nNeedMP = 0;
#endif
			}
			if (chMP->m_mp <= nNeedMP)
				throw MSG_SKILL_ERROR_INSUFF_MP;
			// 사용 조건 검사 : gp
			if(bIsGuildSkill)
			{
				nNeedGP = levelproto->m_needGP;
				if(charSkill == ch)
				{
					if(ch->m_guildInfo->guild()->GetGuildPoint() <= nNeedGP)
						throw MSG_SKILL_ERROR_INSUFF_GP;
				}
				// 사용 조건 검사 : 사용자가 길드마스터인지 확인.
				if(skill->m_proto->m_targetType != STT_GUILD_MEMBER_SELF &&
					ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
					throw MSG_SKILL_ERROR_NOT_GUILD_MASTER;
			}

			// 사용 조건 검사 : use state
			switch (skill->m_proto->CheckUseState(charSkill))
			{
			case 0:
				break;
			case SCT_DEATH:
				throw MSG_SKILL_ERROR_INVAL_STATE_DEATH;
			case SCT_WEAPON:
				throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
			case SCT_SHIELD:
				throw MSG_SKILL_ERROR_INVAL_STATE_SHIELD;
			case SCT_SITDOWN:
				throw MSG_SKILL_ERROR_INVAL_STATE_STAND;
			case SCT_STAND:
				throw MSG_SKILL_ERROR_INVAL_STATE_SITDOWN;
			case SCT_PEACEZONE:
				throw MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE;

			case SCT_DARKNESS:
				throw MSG_SKILL_ERROR_INVAL_STATE_DARKNESS;

			default:
				throw MSG_SKILL_ERROR_INVAL_STATE_MAGIC;
			}

			if (skill->m_proto->CheckSorcererFlag(SSF_USE_ALL))
			{
				switch (charSkill->m_type)
				{
				case MSG_CHAR_PC:
					switch (ch->m_evocationIndex)
					{
					case EVOCATION_NONE:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_CHARACTER))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					case EVOCATION_HELLOUND:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_HELLOUND))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					case EVOCATION_ELENEN:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_ELENEN))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					default:
						throw MSG_SKILL_ERROR_NOTFOUND;
					}
					break;

				case MSG_CHAR_PET:
				case MSG_CHAR_NPC:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_CHARACTER))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;

				case MSG_CHAR_ELEMENTAL:
					switch (pElemental->GetElementalType())
					{
					case ELEMENTAL_FIRE:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_FIRE))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					case ELEMENTAL_WIND:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_WIND))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					case ELEMENTAL_EARTH:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_EARTH))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					case ELEMENTAL_WATER:
						if (!skill->m_proto->CheckSorcererFlag(SSF_USE_WATER))
							throw MSG_SKILL_ERROR_NOTFOUND;
						break;
					default:
						throw MSG_SKILL_ERROR_NOTFOUND;
					}
					break;

				default:
					throw MSG_SKILL_ERROR_NOTFOUND;
				}
			}

			// 타겟 검사
			// 거리 검사
			switch (skill->m_proto->m_targetType)
			{
			case STT_GUILD_ONE:
			case STT_TARGET_ONE:
			case STT_TARGET_RANGE:
			case STT_TARGET_D120:
			case STT_TARGET_RECT:
			case STT_PARTY_ONE:
			case STT_ELEMENTAL_ONE:
				tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->targetIndex, (MSG_CHAR_TYPE)packet->targetType, true);
				if (tch == NULL)
					throw MSG_SKILL_ERROR_INVAL_TARGET;

#ifdef BUGFIX_USE_SKILL_TO_NPC
				if( IS_NPC(tch) )
				{
					CNPC* pNPC = NULL;
					pNPC = TO_NPC(tch);
					if( (pNPC != NULL) &&
						(pNPC->m_flag & (NPC_PEACEFUL | NPC_WARCASTLE | NPC_LORD_SYMBOL | NPC_CASTLE_TOWER)) )
					{
						// 대쉬와 텔레키네시스 스킬은 공성관련 NPC와 평화NPC들에게 사용할 수 없다.
						if( packet->skillIndex == 169 ||
							packet->skillIndex == 401 )
						{
							throw MSG_SKILL_ERROR_INVAL_TARGET;
						}
					}
				}
#endif
				for (i = 0; i < MAX_SKILL_MAGIC; i++)
				{
					if (levelproto->m_magic[i] && levelproto->m_magic[i]->m_levelproto[i])
					{
						if ( IS_NPC(tch) )
						{
							CNPC* pNPC = TO_NPC(tch);

							// npc가 이미 영혼을 빼앗겼으면 더이상 뺏지 못한다.
							// 이 검사가 우선 되어야 한다. 그래서 for문을 먼저 돌면서 검사한다. 아래와 묶여있으면 에러난다.
							if (pNPC->Check_MobFlag(STATE_MONSTER_NO_SOUL) && levelproto->m_magic[i]->m_type == MT_OTHER)
							{
								if (levelproto->m_magic[i]->m_subtype == MST_OTHER_SOUL_DRAIN)
									throw MSG_SKILL_ERROR_DONT_HAS_SOUL;
							}
						}
					}
				}

				for (i = 0; i < MAX_SKILL_MAGIC; i++)
				{
					if (levelproto->m_magic[i] && levelproto->m_magic[i]->m_levelproto[i])
					{
						if ( IS_NPC(tch) )
						{
							CNPC* pNPC = TO_NPC(tch);

							// 테이밍이 되었거나 혼란(혼돈,스턴) 중인 몬스터에게는 어떤 스킬도 사용할 수 없다.
							if ( !pNPC
								|| pNPC->Check_MobFlag(STATE_MONSTER_TAMING)
								|| pNPC->Check_MobFlag(STATE_MONSTER_CONFUSION)
								|| pNPC->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF)
								|| pNPC->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK) )
								throw MSG_SKILL_ERROR_INVAL_TARGET;

							// 코프 오브 나이트 걸리면 다른 스킬이 들어갈 수 없다.
							if (pNPC->m_assist.FindByType(MT_ASSIST, MST_ASSIST_FREEZE))
								throw MSG_SKILL_ERROR_INVAL_TARGET;

							if (levelproto->m_magic[i]->m_type == MT_ASSIST)
							{
								if (levelproto->m_magic[i]->m_subtype == MST_ASSIST_TAMING)
								{
									// npc가 보스급이면 테이밍을 못한다.
#ifdef BUGFIX_USE_SKILL_TO_NPC
									if(pNPC->m_flag & (NPC_BOSS | NPC_MBOSS | NPC_RAID | NPC_PEACEFUL | NPC_WARCASTLE | NPC_LORD_SYMBOL | NPC_CASTLE_TOWER))
#else
									if(pNPC->m_flag & (NPC_BOSS | NPC_MBOSS | NPC_RAID))
#endif
										throw MSG_SKILL_ERROR_INVAL_TARGET;
								}
								// 소울 드레인은 ready에서 부터 dot 데미지가 들어간다.
								if (skill->m_proto->m_index == 661 && levelproto->m_magic[i]->m_subtype == MST_ASSIST_HP_DOT)
								{
									bool bApply = false;
									// [110216:selo] 스킬로 인해 타겟이 죽으면 리턴
									if(0 != ApplySkill(ch, pNPC, skill, -1, bApply) )
										return;
								}
							}
#ifdef BUGFIX_USE_SKILL_TO_NPC
							if( levelproto->m_magic[i]->m_type == MT_OTHER &&
								levelproto->m_magic[i]->m_subtype == MST_OTHER_SOUL_DRAIN )
							{
								if(pNPC->m_flag & (NPC_PEACEFUL | NPC_WARCASTLE | NPC_LORD_SYMBOL | NPC_CASTLE_TOWER))
								{
									throw MSG_SKILL_ERROR_INVAL_TARGET;
								}
							}
#endif
						}

						if (IS_PC(tch))
						{
							// 소울 드레인은 PC에게 사용 못한다.
							if (levelproto->m_magic[i]->m_type == MT_OTHER && levelproto->m_magic[i]->m_subtype == MST_OTHER_SOUL_DRAIN)
								throw MSG_SKILL_ERROR_CANNOT_TO_PC;

							// 테이밍과 혼돈은 PC에게 사용 못한다.
							if(levelproto->m_magic[i]->m_type == MT_ASSIST)
							{
								if (levelproto->m_magic[i]->m_subtype == MST_ASSIST_TAMING
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_CONFUSED
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_MERCENARY
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_TRAP
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_SUICIDE
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_SOUL_TOTEM_BUFF
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_SOUL_TOTEM_ATTK
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_TOTEM_ITEM_BUFF
									|| levelproto->m_magic[i]->m_subtype == MST_ASSIST_TOTEM_ITEM_ATTK)
									throw MSG_SKILL_ERROR_CANNOT_TO_PC;
							}
						}
					} // if
				} // for

				switch (charSkill->m_type)
				{
				case MSG_CHAR_PC:
				case MSG_CHAR_PET:
				case MSG_CHAR_ELEMENTAL:
					if( !ch->IsEnemy(tch) && (skill->m_proto->m_flag & SF_NOTHELP) && ch->CanPvP(tch, false))
						throw MSG_SKILL_ERROR_TARGET_PKLIMITLEVEL;
					if(skill->m_proto->m_targetType == STT_GUILD_ONE)
					{
						CPC* GuildTargetPC = NULL;
						if(IS_PC(tch))
							GuildTargetPC = TO_PC(tch);
						else
							throw MSG_SKILL_ERROR_INVAL_TARGET;

						if( ch->m_guildInfo == NULL || GuildTargetPC->m_guildInfo == NULL ||
							(ch->m_guildInfo->guild()->index() != TO_PC(tch)->m_guildInfo->guild()->index()) )
							throw MSG_SKILL_ERROR_INVAL_TARGET;
					}
					if (skill->m_proto->m_targetType == STT_PARTY_ONE)
					{
						if ( IS_PC(tch) &&
							!( ch->IsParty() && TO_PC(tch)->IsParty() && ch->m_party == TO_PC(tch)->m_party ) &&
							!( ch->IsExped() && TO_PC(tch)->IsExped() && ch->m_Exped == TO_PC(tch)->m_Exped ) )
							throw MSG_SKILL_ERROR_INVAL_TARGET;
					}

					// 소환수 전용 스킬 타입이면
					else if (skill->m_proto->m_targetType == STT_ELEMENTAL_ONE)
					{
						// 대상이 소환수여야 하고
						if (!IS_ELEMENTAL(tch))
							throw MSG_SKILL_ERROR_INVAL_TARGET;
						CElemental* pElemental = TO_ELEMENTAL(tch);
						// 대상의 주인PC가 있어야 하고
						CPC* pTargetPC = pElemental->GetOwner();
						if (pTargetPC == NULL)
							throw MSG_SKILL_ERROR_INVAL_TARGET;
						// 대상의 주인과 스킬 시전자가 동일
						if (pTargetPC != ch/* && (!pTargetPC->IsParty() || !ch->IsParty() || pTargetPC->m_party != ch->m_party*/)
							throw MSG_SKILL_ERROR_INVAL_TARGET;
					}
					break;

				case MSG_CHAR_NPC:
					if (skill->m_proto->m_targetType == STT_PARTY_ONE)
					{
						if (!(IS_NPC(tch) && pNPC->m_proto->m_family != -1 && pNPC->m_proto->m_family == TO_NPC(tch)->m_proto->m_family))
							return ;
					}
					break;

				case MSG_CHAR_APET:
					{
					}
					break;

				default:
					break;
				}

				if (skill->m_proto->GetFireRange(charSkill) * 2 < GetDistance(charSkill, tch))
					throw MSG_SKILL_ERROR_LONG;

				break;
			}
		}		

		// 워프 스킬을 위한 캐릭터 좌표 데이터 설정
		char outY = 0;

		// 클라이언트가 보내줬으면, 워프 스킬이 맞다면
		if (packet->cMoveChar && levelproto->m_magic[0] && levelproto->m_magic[0]->m_type == MT_OTHER
				&& levelproto->m_magic[0]->m_subtype == MST_OTHER_WARP)
		{
			RefMsg(msg) >> packet->fx
						>> packet->fz
						>> packet->fh
						>> packet->fr
						>> packet->cYlayer;

			// 조건 검사 하고 좌표를 저장.. skill fire 에서 저장한 좌표로 워프
			if (ch->m_pArea && ch->m_pArea->IsNotBlocked(ch, packet->cYlayer, packet->fx, packet->fz, true, outY))
			{
				ch->m_pos.m_x = packet->fx;
				ch->m_pos.m_z = packet->fz;
				ch->m_pos.m_h = packet->fh;
				ch->m_pos.m_r = packet->fr;
				ch->m_pos.m_yLayer = packet->cYlayer;
			}
			else
				throw MSG_SKILL_ERROR_WARP_CAN_NOT_GO_THERE;
		}

		// 스킬 시전 상태로
		charSkill->CancelInvisible();
		skill->Ready(tch);
		charSkill->m_currentSkill = skill;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillReadyMsg(rmsg, charSkill, skill, tch);
			charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
		}
	}
	catch (MSG_SKILL_ERROR_TYPE ex)
	{
		if (packet->charType == MSG_CHAR_NPC)
			return;

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillErrorMsg(rmsg, ex, packet->skillIndex, nFailExtraInfo);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_SkillCancel(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::skillCancel* packet = reinterpret_cast<RequestClient::skillCancel*>(msg->m_buf);

	CCharacter* charSkill = NULL;

	RefMsg(msg) >> packet->charType
				>> packet->charIndex;

	LOG_INFO("SKILL_CANCEL_1. charType : %d, charIndex : %d", packet->charType, packet->charIndex);

	switch (packet->charType)
	{
	case MSG_CHAR_NPC:
		if (ch->m_pArea == NULL)
			return ;
		charSkill = ch->m_pArea->FindCharInCell(ch, packet->charIndex, (MSG_CHAR_TYPE)packet->charType);
		break;

	case MSG_CHAR_PET:
		// 펫의 스킬을 PC가 사용
		charSkill = ch;
		break;

	case MSG_CHAR_PC:
		charSkill = ch;
		break;

	case MSG_CHAR_ELEMENTAL:
		charSkill = ch->m_pArea->FindCharInCell(ch, packet->charIndex, (MSG_CHAR_TYPE)packet->charType);
		if (charSkill)
		{
			if (TO_ELEMENTAL(charSkill)->GetOwner() != ch)
				return ;
		}
		break;

	case MSG_CHAR_APET :
		charSkill = ch;
		break;

	default:
		break;
	}

	if(charSkill == NULL)
		return;

	LOG_INFO("SKILL_CANCEL_2. charType : %d, charIndex : %d", charSkill->m_type, charSkill->m_index);

	// 스킬 사용 중인가? 사용중이라면 시전상태인가?
	if (charSkill->m_currentSkill == NULL || charSkill->m_currentSkill->m_state != SKILL_CON_READY)
		return ;

	// 시전 취소
	charSkill->m_currentSkill->Cancel(charSkill);
	charSkill->m_currentSkill = NULL;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillCancelMsg(rmsg, charSkill);
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	}
	LOG_INFO("SKILL_CANCEL_SUCCESS. charType : %d, charIndex : %d", charSkill->m_type, charSkill->m_index);
}

void do_SkillFire(CPC* ch, CNetMsg::SP& msg)
{
	bool bCheck = true;

	if (ch->m_pArea == NULL)
		return ;

	RequestClient::skillFire* packet = reinterpret_cast<RequestClient::skillFire*>(msg->m_buf);

	if (msg->getSize() != (sizeof(RequestClient::skillFire) + (packet->listCount * sizeof(RequestClient::skillFire::tag_list))))
	{
		LOG_ERROR("HACKING : invalid packet size[%d]. charIndex[%d]", msg->getSize(), ch->m_index);
		ch->m_desc->Close("invalid packet size");
		return;
	}

	if (packet->listCount > 0)
	{
		// multi target 중복 검사
		std::set<LONGLONG> tset;
		for (int i = 0; i < packet->listCount; ++i)
		{
			if (tset.insert((LONGLONG)packet->list[i].mtargettype << 32 | packet->list[i].mtargetindex).second == false)
			{
				LOG_ERROR("HACKING : target is duplicating. charIndex[%d]", ch->m_index);
				ch->m_desc->Close("target is duplicating");
				return;
			}
		}
	}

	CSkill* skill = NULL;
	const CSkillProto* proto = NULL;
	const CSkillLevelProto* levelproto = NULL;
	const CSkillLevelProto* level1 = NULL;
	item_search_t vec[MAX_SKILL_NEED_ITEM];
	CCharacter* tch = NULL;
	GoldType_t money = 0;

	float fx = GET_X(ch);
	float fz = GET_Z(ch);
	float fh = GET_H(ch);
	float fr = GET_R(ch);
	char cYlayer = GET_YLAYER(ch);

	int nNeedMP = 0;
	int nNeedGP = 0;
	bool bGuildSkill = false;

	int i;

	CCharacter* chMP;
	CCharacter* charSkill = NULL;

	switch (packet->charType)
	{
	case MSG_CHAR_NPC:
		if (!ch->m_pZone->IsPersonalDungeon())
			return ;
		charSkill = ch->m_pArea->FindCharInCell(ch, packet->charIndex, (MSG_CHAR_TYPE)packet->charType);
		break;

	case MSG_CHAR_PET:
		if (ch->m_bImmortal || ch->IsDisable())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		charSkill = ch;
		break;

	case MSG_CHAR_PC:
		if ( (ch->m_bImmortal && ch->m_bFirstDoIt == false) || ch->IsDisable() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		charSkill = ch;
		if (packet->charIndex != ch->m_index)
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		if (ch->m_bImmortal || ch->IsDisable())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, packet->skillIndex, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		charSkill = ch->GetElemental(packet->charIndex);
		break;
	case MSG_CHAR_APET:
		{
			CAPet* apet = ch->GetAPet();
			if( apet )
			{
				if( apet->IsSummon() )
				{
					charSkill = apet;
				}
				break;
			}
			return;
		}
		break;

	default:
		return ;
	}

	if (!charSkill)
		return ;

	// 스킬 시전중 검사
	if (charSkill->m_currentSkill == NULL
			|| charSkill->m_currentSkill->m_proto->m_index != packet->skillIndex
			|| charSkill->m_currentSkill->m_state != SKILL_CON_READY)
		goto INSUFF;

	if ((skill = charSkill->m_currentSkill) == NULL)
		goto INSUFF;
	if ((proto = skill->m_proto) == NULL)
		goto INSUFF;
	if ((levelproto = proto->Level(skill->m_level)) == NULL)
		goto INSUFF;

	if (proto->m_levelproto[skill->m_level - 1]->m_targetNum < packet->listCount)
		packet->listCount = proto->m_levelproto[skill->m_level - 1]->m_targetNum;

	if (packet->cMoveChar)
	{
		fx = packet->fx;
		fz = packet->fz;
		fh = packet->fh;
		fr = packet->fr;
		cYlayer = packet->cYlayer;
	}

	if( gserver->m_pulse < (skill->m_usetime + ((skill->m_proto->m_readyTime * (100 - ch->m_skillSpeed) / 100))) )
	{
		//해킹 시도 클라이언트 접속 종료시켜버린다.
		ch->m_desc->IncreaseHackCount(1);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			std::string buffer = boost::str(boost::format("HackCount = %d.") % ch->m_desc->m_hackCount);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", buffer.c_str());
			SEND_Q(rmsg, ch->m_desc);
		}	
	}
	
	if( (skill->m_proto->m_flag & SF_TOGGLE) && skill->m_toggle == true )
	{
		bCheck = false;
	}

	if( skill->m_proto->m_index == SKILL_GUILD_ROOM_RECALL
		|| skill->m_proto->m_index == SKILL_GUILD_ROOM_WARP)
	{
		bCheck = false;
	}

	if(bCheck == true)
	{
		if (IS_PC(charSkill))
		{
			// 사용 조건 : 스탯 : 스킬 레벨 설정에서 1레벨 기준
			level1 = skill->m_proto->Level(1);
			if (!level1)
				goto INSUFF;
			if (ch->m_statpt_str < level1->m_learnstr)
				goto INSUFF;
			if (ch->m_statpt_dex < level1->m_learndex)
				goto INSUFF;
			if (ch->m_statpt_int < level1->m_learnint)
				goto INSUFF;
			if (ch->m_statpt_con < level1->m_learncon)
				goto INSUFF;

			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;

				//나스 소모스킬 사용조건 검사( 나스 인벤토리 제거 패치)
				if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() < levelproto->m_needItemCount[i])
					goto INSUFF;
				else if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() >= levelproto->m_needItemCount[i])
				{
					money = levelproto->m_needItemCount[i];
					continue;
				}

				int sc = ch->m_inventory.searchItemByCondition(levelproto->m_needItemIndex[i], 0, 0, vec[i]);
				if (sc == 0 || sc < levelproto->m_needItemCount[i])
					goto INSUFF;
			}
		}
		else if ( IS_APET(charSkill) )
		{
			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;

				//나스 소모스킬 사용조건 검사( 나스 인벤토리 제거 패치)
				if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() < levelproto->m_needItemCount[i])
					goto INSUFF;
				else if (levelproto->m_needItemIndex[i] == NAS_ITEM_DB_INDEX && ch->m_inventory.getMoney() >= levelproto->m_needItemCount[i])
				{
					money = levelproto->m_needItemCount[i];
					continue;
				}

				CPC* pOwner = TO_APET(charSkill)->GetOwner();
				if( pOwner )
				{
					int sc = pOwner->m_inventory.searchItemByCondition(levelproto->m_needItemIndex[i], 0, 0, vec[i]);
					if (sc == 0 || sc < levelproto->m_needItemCount[i])
						goto INSUFF;
				}
			}
		}


		// 타겟 검사
		// 이전 타겟이 있고
		if (skill->m_targetIndex != -1)
		{
			// 현재 타겟을 구해서
			tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->targetIndex, (MSG_CHAR_TYPE)packet->targetType, true);
			// 타겟이 없거나 이전과 다르면
			if (tch == NULL || skill->m_targetIndex != packet->targetIndex || skill->m_targetType != packet->targetType)
				goto INSUFF;
		}

		// 소환수는 MP 공유
		chMP = (IS_ELEMENTAL(charSkill)) ? ch : charSkill;

		// 사용 조건 검사 : hp
		if ( levelproto->m_needHP !=0 && charSkill->m_hp <= levelproto->m_needHP)
			goto INSUFF;

		// 사용 조건 검사 : mp
		nNeedMP = levelproto->m_needMP;
		if (charSkill == ch)
		{
			if (ch->m_opDecreaseSkillMP > 0)
				nNeedMP -= nNeedMP * ch->m_opDecreaseSkillMP / 100;
#ifdef ASSIST_DECREASE_SKILL_MP
			if (ch->m_assist.m_avRate.decreaseskillmp > 0)
				nNeedMP -= nNeedMP *
				ch->m_assist.m_avRate.decreaseskillmp / 100;
			if (ch->m_assist.m_avAddition.decreaseskillmp > 0)
				nNeedMP -= ch->m_assist.m_avAddition.decreaseskillmp;

			if (nNeedMP < 0)
				nNeedMP = 0;
#endif
		}
		if (chMP->m_mp <= nNeedMP)
			goto INSUFF;

		// 스킬fire GP감소
		if(proto->m_flag & SF_GUILD)
		{
			nNeedGP = levelproto->m_needGP;
			if(!ch->m_guildInfo)
				goto INSUFF;
			if(!ch->m_guildInfo->guild())
				goto INSUFF;
			if(!ch->m_guildInfo->guild()->boss())
				goto INSUFF;
			if(skill->m_proto->m_targetType != STT_GUILD_MEMBER_SELF &&
				ch->m_guildInfo->guild()->boss()->GetPC() != ch)
				goto INSUFF;

			if(ch->m_guildInfo->guild()->GetGuildPoint() <= nNeedGP)
				goto INSUFF;
			else
				bGuildSkill = true;
		}

		// 사용 조건 검사 : use state
		// 하이드는 파이어 행동시 검사하지 않음
		if( skill->m_proto->m_index == 237
			|| skill->m_proto->m_index == 236
			|| skill->m_proto->m_index == 1542
			|| skill->m_proto->m_index == 1552) ;
		else
		{
			switch (skill->m_proto->CheckUseState(charSkill))
			{
			case 0:
				break;
			default:
				goto INSUFF;
			}
		}

		// 사용 조건 검사 : Soul 사용 여부
		if (IS_PC(charSkill))
		{
			CPC* pPC = TO_PC(charSkill);

			// 영혼이 소모되는 스킬이라면
			if (pPC && pPC->m_desc)
			{
				if(proto->m_soul_consum > 0)
				{
					int soul_count = 0;		// 영혼의 소유 개수
					int remain_soul = 0;	// 영혼의 남은 개수

					soul_count = pPC->GetSoul_Count();
					remain_soul = soul_count - proto->m_soul_consum;

					// 영혼의 개수가 마이너스가 되면 스킬 취소
					if ( remain_soul >= 0 )
						pPC->SetSoul_Count(remain_soul);
					else
					{
						GAMELOG << init("SetSoul_Count MINUS ERROR")
							<< "USER_ID" << delim << pPC->m_desc->m_idname << delim
							<< "USER_INDEX" << delim << pPC->m_desc->m_index << delim << end;

						pPC->SetSoul_Count(0);
						goto INSUFF;
					}

					{
						// 영혼 개수 매세지 보내기
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillSoulCountMsg(rmsg, pPC->GetSoul_Count());
						SEND_Q(rmsg, pPC->m_desc);
					}
				}

				// 지속 스킬을 위해 스킬 타겟 정보를 캐릭터에 저장한다.
				if (packet->listCount > 0)
				{
					pPC->m_skillTargetCount = packet->listCount;
					int a;
					pPC->m_targettype	= new char[packet->listCount];
					pPC->m_targetindex	= new int[packet->listCount];

					for (a = 0; a < packet->listCount; a++)
					{
						pPC->m_targettype[a] = packet->list[a].mtargettype;
						pPC->m_targetindex[a] = packet->list[a].mtargetindex;
					}
				}
			}
		}
	}

	// 스킬 파이어
	skill->Fire();

	if (packet->cMoveChar && tch)
	{
		if (GetDistance(fx, fz, fh, tch) <= CELL_SIZE * 2.0f)
		{
			charSkill->m_pArea->MoveChar(charSkill, cYlayer, fx, fz, fh , fr, MSG_MOVE_STOP, NULL);
		}
	}

	{
		boost::scoped_array<MultiTarget> temp(new MultiTarget[packet->listCount]);

		for (int i = 0; i < packet->listCount; ++i)
		{
			temp[i].mtargettype = packet->list[i].mtargettype;
			temp[i].mtargetindex = packet->list[i].mtargetindex;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillFireMsg(rmsg, charSkill, skill, tch, packet->listCount, temp.get(), packet->cMoveChar, fx, fz, fh, fr, cYlayer);
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	}

	if(bCheck == true)
	{
		// HP 소모, MP 소모
		charSkill->m_hp -= levelproto->m_needHP;
		chMP->m_mp -= nNeedMP;
		if(bGuildSkill)
		{
			ch->m_guildInfo->guild()->AddGuildPoint(-nNeedGP);

			// 헬퍼로 Guild Point 업데이트 발송....
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNewGuildPointUpdate(rmsg, ch->m_index, ch->m_guildInfo->guild()->index(), -nNeedGP);
			SEND_Q(rmsg, gserver->m_helper);
		}
		switch (charSkill->m_type)
		{
		case MSG_CHAR_PC:
		case MSG_CHAR_ELEMENTAL:
			ch->m_bChangeStatus = true;
			break;
		case MSG_CHAR_APET:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExAPetStatusMsg(rmsg, TO_APET(charSkill) );
				charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
				ch->m_bChangeStatus = true;
			}
			break;
		default:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				CharStatusMsg(rmsg, charSkill, 0);
				charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
			}
			break;
		}

		// ITEM 소모
		if (IS_PC(charSkill))
		{
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				if (levelproto->m_needItemCount[i] <= 0)
					continue;

				ch->m_inventory.deleteItem(vec[i], levelproto->m_needItemCount[i]);
			}

			if (money > 0)
			{
				ch->m_inventory.decreaseMoney(money);
				money = 0;
			}
		}
		else if ( IS_APET(charSkill) )
		{
			CPC* pOwner = TO_APET(charSkill)->GetOwner();
			if( pOwner )
			{
				for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
				{
					if (levelproto->m_needItemCount[i] <= 0)
						continue;

					pOwner->m_inventory.deleteItem(vec[i], levelproto->m_needItemCount[i]);
				}

				if (money > 0)
				{
					ch->m_inventory.decreaseMoney(money);
					money = 0;
				}
			}
		}
	}

	// 스킬 적용
	// 타게팅에 따라
	bool bApply;
	switch (proto->m_targetType)
	{
	case STT_SELF_ONE:
		ApplySkill(charSkill, charSkill, skill, -1, bApply);
		break;

	case STT_GUILD_MEMBER_SELF:
		ApplySkill(charSkill, charSkill, skill, -1, bApply);
		break;

	case STT_TARGET_ONE:
		if (tch)
			ApplySkill(charSkill, tch, skill, -1, bApply);
		break;
	case STT_GUILD_SELF_RANGE:
		if(ApplySkill(charSkill, charSkill, skill, -1, bApply) != -1)
		{
			for(i = 0; i < packet->listCount; i++)
			{
				tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->list[i].mtargetindex, (MSG_CHAR_TYPE)packet->list[i].mtargettype, true);
				if (tch == NULL)
					continue;

				if (GetDistance(charSkill, tch) <= proto->m_appRange + 1.0f)
				{
#if defined (LC_GAMIGO)
					// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
					if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && !(tch->GetMapAttr() & MATT_FREEPKZONE) )
						continue;
#endif // LC_GAMIGO
					if(skill->m_proto->m_flag & (1 << 1))
					{
						// HELP고 tch가 나와 같은 길드면 apply보내기.
						CPC* pc = TO_PC(charSkill);
						CPC* tpc = TO_PC(tch);

						if (pc == NULL || tpc == NULL)
							continue;

						if (pc->m_guildInfo == NULL || tpc->m_guildInfo == NULL)
							continue;

						if (pc->m_guildInfo->guild()->index() != tpc->m_guildInfo->guild()->index())
							continue;

						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
					else
					{
						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
				}
			}
		}
		break;
	case STT_SELF_RANGE:
		if (ApplySkill(charSkill, charSkill, skill, -1, bApply) != -1)
		{
			for (i = 0; i < packet->listCount; i++)
			{
				tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->list[i].mtargetindex, (MSG_CHAR_TYPE)packet->list[i].mtargettype, true);
				if (tch == NULL)
					continue;

				if (GetDistance(charSkill, tch) <= proto->m_appRange + 1.0f)
				{
#if defined (LC_GAMIGO)
					// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
					if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && !(tch->GetMapAttr() & MATT_FREEPKZONE) )
						continue;
#endif // LC_GAMIGO

#ifdef QA_RANGESKILL_APET_DAMAGE
					if( (skill->m_proto->m_flag & SF_NOTHELP)
							&& IS_PC(charSkill)
							&& IS_APET(tch)
							&& (!charSkill->IsEnemy(tch)))
						continue;
#endif // QA_RANGESKILL_APET_DAMAGE

					if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
						break;
				}
			}
		}
		break;

	case STT_TARGET_RANGE:
		if (tch)
		{
			float tx = GET_X(tch);
			float tz = GET_Z(tch);
			float th = GET_H(tch);

			if (ApplySkill(charSkill, tch, skill, -1, bApply) != -1)
			{
				for (i = 0; i < packet->listCount; i++)
				{
					tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->list[i].mtargetindex, (MSG_CHAR_TYPE)packet->list[i].mtargettype, true);
					if (tch == NULL)
						continue;

					if (GetDistance(tx, tz, th, tch) <= proto->m_appRange + 1.0f)
					{
#if defined (LC_GAMIGO)
						// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
						if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && !(tch->GetMapAttr() & MATT_FREEPKZONE) )
							continue;
#endif // LC_GAMIGO

#ifdef QA_RANGESKILL_APET_DAMAGE
						if( (skill->m_proto->m_flag & SF_NOTHELP)
								&& IS_PC(charSkill)
								&& IS_APET(tch)
								&& (!charSkill->IsEnemy(tch)))
							continue;
#endif // QA_RANGESKILL_APET_DAMAGE

						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
				}
			}
		}
		break;

	case STT_TARGET_RECT:
	case STT_TARGET_D120:
		if (tch)
		{
			if (ApplySkill(charSkill, tch, skill, -1, bApply) != -1)
			{
				for (i = 0; i < packet->listCount; i++)
				{
					tch = charSkill->m_pArea->FindCharInCell(charSkill, packet->list[i].mtargetindex, (MSG_CHAR_TYPE)packet->list[i].mtargettype, true);
					if (tch == NULL)
						continue;

					if (GetDistance(charSkill, tch) <= proto->m_appRange + 1.0f)
					{
#if defined (LC_GAMIGO)
						// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
						if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && !(tch->GetMapAttr() & MATT_FREEPKZONE) )
							continue;
#endif // LC_GAMIGO

#ifdef QA_RANGESKILL_APET_DAMAGE
						if( (skill->m_proto->m_flag & SF_NOTHELP)
								&& IS_PC(charSkill)
								&& IS_APET(tch)
								&& (!charSkill->IsEnemy(tch)))
							continue;
#endif // QA_RANGESKILL_APET_DAMAGE

						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
				}
			}
		}
		break;

	case STT_GUILD_ONE:
		switch(charSkill->m_type)
		{
		case MSG_CHAR_PC:
			{
				if(tch->m_type != MSG_CHAR_PC)
					break;

				CPC* tpc = TO_PC(tch);
				if(tpc)
				{
					if(ch->m_guildInfo && tpc->m_guildInfo &&
							ch->m_guildInfo->guild()->index() == tpc->m_guildInfo->guild()->index())
					{
						ApplySkill(charSkill, tch, skill, -1, bApply);
					}
				}
			}
			break;

		default:
			break;
		}
		break;
	case STT_PARTY_ONE:
	case STT_ELEMENTAL_ONE:
		switch (charSkill->m_type)
		{
		case MSG_CHAR_PC:
		case MSG_CHAR_PET:
		case MSG_CHAR_ELEMENTAL:
			{
				CPC* tpc = NULL;
				switch (tch->m_type)
				{
				case MSG_CHAR_PC:
					tpc = TO_PC(tch);
					break;

				case MSG_CHAR_PET:
					tpc = TO_PET(tch)->GetOwner();
					break;

				case MSG_CHAR_ELEMENTAL:
					tpc = TO_ELEMENTAL(tch)->GetOwner();
					break;

				default:
					break;
				}
				if (tpc)
				{
					if(skill->m_proto->m_index == 50)						// 힐러 리저렉션 스킬일 때 로그 남기기.
					{
						GAMELOG << init("USE SKILL RESURRECTION")
								<< "USE CHARACTER" << delim
								<< ch->m_index << delim
								<< ch->m_name << delim
								<< ch->m_nick << " => "
								<< "TARGET CHARACTER" << delim
								<< tpc->m_index << delim
								<< tpc->m_name << delim
								<< tpc->m_nick << end;
					}
					if (ch->IsParty() && tpc->IsParty() && ch->m_party == tpc->m_party)
						ApplySkill(charSkill, tch, skill, -1, bApply);

					//원정대 추가
					if (ch->IsExped() && tpc->IsExped() && ch->m_Exped == tpc->m_Exped)
						ApplySkill(charSkill, tch, skill, -1, bApply);
				}
			}
			break;

		case MSG_CHAR_NPC:
			{
				CNPC* tnpc = TO_NPC(tch);
				if (tnpc != NULL)
				{
					if (!(TO_NPC(charSkill)->m_proto->m_family != -1 && TO_NPC(charSkill)->m_proto->m_family == tnpc->m_proto->m_family))
						ApplySkill(charSkill, tch, skill, -1, bApply);
				}
			}
			break;

		default:
			break;
		}
		break;
	case STT_GUILD_ALL:
		switch(charSkill->m_type)
		{
		case MSG_CHAR_PC:
			if(ch->m_guildInfo)
			{
				ApplySkillGuild(ch, skill, -1, bApply);
			}
			break;
		default:
			break;
		}
		break;
	case STT_PARTY_ALL:
		switch (charSkill->m_type)
		{
		case MSG_CHAR_PC:
			if(ch->IsExped())
			{
				ApplySkillExped(ch, skill, -1, bApply);
			}
			else
			{
				ApplySkillParty(ch, skill, -1, bApply);
			}
			break;

		case MSG_CHAR_NPC:
			if (TO_NPC(charSkill)->m_proto->m_family != -1)
			{
				int sx = charSkill->m_cellX - CELL_EXT;
				int ex = charSkill->m_cellX + CELL_EXT;
				int sz = charSkill->m_cellZ - CELL_EXT;
				int ez = charSkill->m_cellZ + CELL_EXT;

				int x, z;

				for (x = sx; x <= ex; x++)
				{
					if (x < 0 || x >= charSkill->m_pArea->m_size[0]) continue;

					for (z = sz; z <= ez; z++)
					{
						if (z < 0 || z >= charSkill->m_pArea->m_size[1]) continue;

						CCharacter *pChar;
						CCharacter *pCharNext = charSkill->m_pArea->m_cell[x][z].m_listChar;

						while ((pChar = pCharNext))
						{
							pCharNext = pCharNext->m_pCellNext;

							if (IS_NPC(pChar) && TO_NPC(pChar)->m_proto->m_family == TO_NPC(charSkill)->m_proto->m_family)
							{
								if (ApplySkill(charSkill, pChar, skill, -1, bApply) == -1)
									break;
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
		break;

	default:
		goto INSUFF;
	}

	goto EXITFUNC;

INSUFF:
	// 시전 취소
	if (charSkill->m_currentSkill)
	{
		charSkill->m_currentSkill->Cancel(charSkill);
		charSkill->m_currentSkill = NULL;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillCancelMsg(rmsg, charSkill);
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	}

EXITFUNC:
	// 불의정령이 Soul of Fire 시전하면 불의 정령 사망
	if (packet->skillIndex == 324)
	{
		if (IS_ELEMENTAL(charSkill))
		{
			ch->UnsummonElemental(TO_ELEMENTAL(charSkill));
		}
	}

#ifdef ENABLE_ROGUE_SKILL125_BRZ
	if(packet->skillIndex == 125)
	{
		if(!bApply)
		{
			// 로그 와일드 라쏘 스킬 실패
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_MISS_ROGUE_SKILL, packet->skillIndex, 0);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
#endif // ENABLE_ROGUE_SKILL125_BRZ
}

#ifdef HANARO_EVENT
void do_SkillHanaroEvent(CPC * ch, CNetMsg::SP& msg)
{
#if defined (LC_RUS)
	{} // 러시아는 전 채널
#elif defined(LC_KOR)
	if( gserver->m_subno != 1)		// 초보지원 NPC 1채널만
	{
		return;
	}
#elif defined (LC_USA) || defined (LC_BILA) || defined (LC_GAMIGO) || defined (LC_RUS)
	if( gserver->m_subno != 2)		// 초보지원 NPC 2채널만
	{
		return;
	}
#else
	if( gserver->m_subno != 3)		// 초보지원 NPC 3채널만
	{
		return;
	}
#endif // defined (LC_RUS)

	RequestClient::skillHanaroEvent* packet = reinterpret_cast<RequestClient::skillHanaroEvent*>(msg->m_buf);
	CNPC* pNpc = (CNPC*)ch->m_pArea->FindCharInCell(ch, packet->npcIndex, MSG_CHAR_NPC, false);
	if (pNpc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_ERROR_NOT_EXIST_NPC, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (pNpc->IsFlag(NPC_EVENT) == 0)
	{
		LOG_ERROR("HACKING : not equal npc flag. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not equal npc flag");
		return ;
	}

	if (ch->m_level > 60)
	{
		// 레벨제한으로 인한 버프를 받지 못함.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_MIDLEVEL_SUPPORT, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CSkill * pSkill = gserver->m_skillProtoList.Create(654, 1) ;
	if (pSkill && ch->CanApplySkill(pSkill->m_proto, pSkill->m_proto->Level(pSkill->m_level)))
	{
		bool bApply = false;
		ApplySkill(ch, ch, pSkill, -1 , bApply);
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_MIDLEVEL_SUPPORT, 1);
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif

void do_SealSkillLearn(CPC* ch, CNetMsg::SP& msg)
{
	try
	{
		RequestClient::sealSkillLearn* packet = reinterpret_cast<RequestClient::sealSkillLearn*>(msg->m_buf);

		CNPC* pNpc = (CNPC*)ch->m_pArea->FindCharInCell(ch, packet->npcIndex, MSG_CHAR_NPC, false);
		if (pNpc == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_ERROR_NOT_EXIST_NPC, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		const CSkillProto* proto = gserver->m_skillProtoList.Find(packet->skillIndex);
		if (proto == NULL)
		{
			LOG_ERROR("HACKING : not found skill[%d]. charIndex[%d]", packet->skillIndex, ch->m_index);
			ch->m_desc->Close("not found skill");
			return;
		}

		if (!proto->IsSeal())
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		if (ch->m_sealSkillList.count() >= MAX_SEAL_TYPE_SKILL)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		// 이미 배웠으면 더 배울 필요없다.
		CSkill* skill = ch->m_sealSkillList.Find(packet->skillIndex);
		if (skill)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		//const CSkillLevelProto* levelproto = proto->Level(skill->m_level);
		const CSkillLevelProto* levelproto = proto->Level(1);
		if (!levelproto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		// 레벨
		if (levelproto->m_learnLevel > ch->m_level)
			throw MSG_SKILL_LEARN_ERROR_LEVEL;

		// 아이템 검사
		int i;
		item_search_t vec[MAX_SKILL_LEARN_ITEM];
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if (levelproto->m_learnItemIndex[i] <= 0)
				continue ;

			if(levelproto->m_learnItemIndex[i] == NAS_ITEM_DB_INDEX)
			{
				if(ch->m_inventory.getMoney() < levelproto->m_learnItemCount[i])
					throw MSG_SKILL_LEARN_ERROR_ITEM;
			}
			else
			{
				int sc = ch->m_inventory.searchItemByCondition(levelproto->m_learnItemIndex[i], 0, 0, vec[i]);
				if (sc == 0 || sc < levelproto->m_learnItemCount[i])
					throw MSG_SKILL_LEARN_ERROR_ITEM;
			}
		}

		// 스킬 검사
		for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
		{
			if (levelproto->m_learnSkillIndex[i] < 0)
				continue ;
			const CSkillProto* lproto = gserver->m_skillProtoList.Find(levelproto->m_learnSkillIndex[i]);
			CSkill* lskill = NULL;
			if (lproto->IsSeal())
				lskill = ch->m_sealSkillList.Find(levelproto->m_learnSkillIndex[i]);
			if (lskill == NULL || lskill->m_level < levelproto->m_learnSkillLevel[i])
				throw MSG_SKILL_LEARN_ERROR_SKILL;
		}

		// 스킬 추가
		if (skill == NULL)
		{
			skill = gserver->m_skillProtoList.Create(packet->skillIndex);
			if (!skill)
				throw MSG_SKILL_LEARN_ERROR_SYSTEM;
			if (proto->IsSeal())
			{
				ch->m_sealSkillList.Add(skill);
				ch->AddSealSkill(packet->skillIndex);
			}
		}

		// 아이템 소모
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if(levelproto->m_learnItemIndex[i] == NAS_ITEM_DB_INDEX)
			{
				ch->m_inventory.decreaseMoney(levelproto->m_learnItemCount[i]);
			}
			else
			{
				ch->m_inventory.deleteItem(vec[i], levelproto->m_learnItemCount[i]);
			}
		}

		GAMELOG << init("SEAL SKILL LEARN", ch)
				<< skill->m_proto->m_index
				<< delim
				<< skill->m_level
				<< end;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSealSkillLeranMsg(rmsg, ch, skill);
			SEND_Q(rmsg, ch->m_desc);
		}

		// 연합 등록 증표 이면 Action 추가
		if (skill->m_proto->m_index == FACTORY_UNION_REG_SKILL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAction(rmsg, ch, ACTION_GENERAL, AGT_FACTORY);
			SEND_Q(rmsg, ch->m_desc);
		}

		//ch->CalcStatus(true);
	}
	catch (MSG_SKILL_LEARN_ERROR_TYPE learnerr)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillLearnErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_SkillMidLevelSupport(CPC* ch, CNetMsg::SP& msg)
{
	if(ch->m_level < 61 || ch->m_level > 90)
	{
		// 레벨제한으로 인한 버프를 받지 못함.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_MIDLEVEL_SUPPORT, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CSkill * pSkill = gserver->m_skillProtoList.Create(1002, 1) ;									// SKILL INDEX
	if (pSkill == NULL)
	{
		LOG_ERROR("not found skill[%d]. charIndex[%d]", 1002, ch->m_index);
		return;
	}

	if (ch->CanApplySkill(pSkill->m_proto, pSkill->m_proto->Level(pSkill->m_level)))
	{
		bool bApply = false;
		ApplySkill(ch, ch, pSkill, -1 , bApply);
		// 버프 받음.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillSupportMsg(rmsg, MSG_SKILL_MIDLEVEL_SUPPORT, 1);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_Skill_UseEP ( CPC* ch, CNetMsg::SP& msg )
{
	if(ch->m_pArea->m_zone->isRVRZone())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_ZONE, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(ch->holy_water_item != NULL)
	{
		//EP 스킬을 사용하면 성수아이템을 해제 한다.
		ch->changeToggleState(ch->holy_water_item->getVIndex(), TOGGLE_ITEM);
		ch->SendHolyWaterStateMsg(NULL);
	}

	enum { ARRAY_DATA_SIZE = 6 };
	static const int useLevelPoint[ARRAY_DATA_SIZE] = { 0, 50000, 110000, 180000, 250000, 350000 };
	static const int useSkill[ARRAY_DATA_SIZE][2] = { {0,0}, {1062,1}, {1062,2}, {1063,1}, {1064,1}, {1065,1} };
	int useLevel = 0;

	// 사용할 스킬, 레벨을 확인
	while ( useLevel < ARRAY_DATA_SIZE )
	{
		useLevel++;
		if ( useLevelPoint[useLevel] > ch->m_ep )
		{
			break;
		}
	}
	useLevel--;

	if( useLevel < 1 || useLevel > 5 )
	{
		return;
	}

	// 스킬 사용
	bool bApply;
	CSkill * pSkill = gserver->m_skillProtoList.Create( useSkill[useLevel][0], useSkill[useLevel][1] );
	{
		ApplySkill(ch, ch, pSkill, -1, bApply);
		if( !bApply && pSkill )
		{
			delete pSkill;
			pSkill = NULL;
		}
	}

	// 남은 AP Update
	if( bApply )
	{
		ch->AddEP( -useLevelPoint[useLevel] );
		GAMELOG << init("EP SKILL START", ch) << "Lv" << useLevel << delim
				<< "Skill Index" << delim << pSkill->m_proto->m_index << delim
				<< "Skill Level" << delim << pSkill->m_level << end;
	}
}
