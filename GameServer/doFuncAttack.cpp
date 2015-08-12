#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Battle.h"
#include "DratanCastle.h"
#include "../ShareLib/packetType/ptype_old_do_attack.h"

// 믈리 어택
void do_pd_Attack(CPC* pc, CNetMsg::SP& msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckRespond(pc);

	RequestClient::doPDAttack* packet = reinterpret_cast<RequestClient::doPDAttack*>(msg->m_buf);

	if (packet->multicount > 20)
	{
		LOG_ERROR("HACKING : invalid multi count[%d]. charIndex[%d]", packet->multicount, pc->m_index);
		pc->m_desc->Close("invalid multi count");
		return;
	}

	// multi target의 중복 검사
	if (packet->multicount > 1)
	{
		std::set<int> tset;
		for (int i = 0; i < packet->multicount; ++i)
		{
			if (tset.insert(packet->list[i].index).second == false)
			{
				LOG_ERROR("HACKING : duplicate multi target[%d]. charIndex[%d]", packet->list[i].index, pc->m_index);
				pc->m_desc->Close("duplicate multi target");
				return;
			}
		}
	}

	// 대상 검색 : 인접 셀에서만
	CArea* area = pc->m_pArea;
	if (area == NULL)
	{
		LOG_ERROR("HACKING : not found area. charIndex[%d]", pc->m_index);
		pc->m_desc->Close("not found area");
		return;
	}

	CCharacter* tch = area->FindCharInCell(pc, packet->tIndex, (MSG_CHAR_TYPE)packet->tCharType);
	if (tch == NULL)
		return;

	int preIndex = -1;
	for (int i = 0; i < packet->multicount; ++i)
	{
		if(preIndex == packet->list[i].index)
		{
			// 가까운 마을로
			int nearZone;
			int nearZonePos;
			CZone* pZone = gserver->FindNearestZone(pc->m_pZone->m_index, GET_X(pc), GET_Z(pc), &nearZone, &nearZonePos);
			if (pZone == NULL)
				return;

			GoZone(pc, nearZone,
				   pZone->m_zonePos[nearZonePos][0],															// ylayer
				   GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z

			return;
		}

		preIndex = packet->list[i].index;

		CCharacter* ch = area->FindCharInCell(pc, packet->list[i].index, (MSG_CHAR_TYPE) MSG_CHAR_NPC);

		if(!ch) continue;

		if( !IS_NPC(ch) )
		{
			CPC* bugPC = NULL;
			if( IS_ELEMENTAL(ch) )
			{
				CElemental *ele = TO_ELEMENTAL(ch);
				bugPC = ele->GetOwner();
			}
			if( IS_PET(ch) )
			{
				CPet* pet = TO_PET(ch);
				bugPC = pet->GetOwner();
			}

			if( IS_PC(ch) )
			{
				bugPC = TO_PC(ch);
			}

			if( !bugPC )
				return;

			// 가까운 마을로
			int nearZone;
			int nearZonePos;
			CZone* pZone = gserver->FindNearestZone(bugPC->m_pZone->m_index, GET_X(bugPC), GET_Z(bugPC), &nearZone, &nearZonePos);
			if (pZone == NULL)
				return;

			GoZone(bugPC, nearZone,
				   pZone->m_zonePos[nearZonePos][0],															// ylayer
				   GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z

			GAMELOG << init("PD_BUG", bugPC)
					<< end;
			return;
		}

		int ret = ProcAttack(ch, tch, ch->GetAttackType(NULL), NULL, 0);
		if (ret == -1)
			return ;
	}
}

void do_Attack(CPC* pc, CNetMsg::SP& msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckRespond(pc);

	RequestClient::doAttack* packet = reinterpret_cast<RequestClient::doAttack*>(msg->m_buf);

	// 동시 공격은 최대 5
	if (packet->multicount > 5)
	{
		LOG_ERROR("HACKING : invalid multi count[%d]. charIndex[%d]", packet->multicount, pc->m_index);
		pc->m_desc->Close("invalid multi count");
		return;
	}

	// multi target의 중복 검사
	if (packet->multicount > 1)
	{
		std::set<int> tset;
		for (int i = 0; i < packet->multicount; ++i)
		{
			if (tset.insert(packet->list[i].index).second == false)
			{
				LOG_ERROR("HACKING : duplicate multi target[%d]. charIndex[%d]", packet->list[i].index, pc->m_index);
				pc->m_desc->Close("duplicate multi target");
				return;
			}
		}
	}

	// 대상 검색 : 인접 셀에서만
	CArea* area = pc->m_pArea;
	if (area == NULL)
		return;

	CCharacter* ch = area->FindCharInCell(pc, packet->aIndex, (MSG_CHAR_TYPE)packet->aCharType);
	if (ch == NULL)
		return;

	CCharacter* tch = area->FindCharInCell(ch, packet->tIndex, (MSG_CHAR_TYPE)packet->tCharType);
	if (tch == NULL)
		return;

	// 공격자가 PC이면 자신의 캐릭만 조정
	if (IS_PC(ch) && ch != pc)
		return ;

	switch (ch->m_type)
	{
	case MSG_CHAR_PC:
		{
			if( IS_NPC(tch))
			{
				CPC * pPC = TO_PC(ch);
				CNPC * pNPC = TO_NPC(tch);
				if( pPC->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == pNPC )
					return;
			}

			//pvp보호 아이템 체크
			if (checkPvPProtect(pc, tch) == false)
				return;

			// 공격 거리 검사
			if (GetDistance(ch, tch) > ch->m_attackRange * 2)
				return ;

			// 공속 검사
			if ( ch->ChekAttackType() && ch->CheckHackAttack(pc))
				return ;

			// 펫 타고 있으면 불가능
			if (pc->GetPet() && pc->GetPet()->IsMount())
				return ;

			// 공성 아이템 착용시 해당 스킬 발동
			int mixSkillIndex[] = { pc->m_opSturnIndex, pc->m_opBloodIndex, pc->m_opPoisonIndex, pc->m_opSlowIndex, pc->m_opMoveIndex };
			int mixSkillLevel[] = { pc->m_opSturnLevel, pc->m_opBloodLevel, pc->m_opPoisonLevel, pc->m_opSlowLevel, pc->m_opMoveLevel };
			CSkill* skillMixItem = NULL;
			int i;
			int bStop = 0;
			for (i = 0; i < 5; i++)
			{
				if (mixSkillIndex[i] > 0 && mixSkillLevel[i] > 0)
				{
					skillMixItem = gserver->m_skillProtoList.Create(mixSkillIndex[i], mixSkillLevel[i]);
					if (skillMixItem)
					{
						bool bApply;
						bStop = ApplySkill(ch, tch, skillMixItem, -1, bApply);
					}
					delete skillMixItem;
					skillMixItem = NULL;
					if (bStop != 0)
						return ;
				}
			}

			if (IS_PC(ch))
			{
				CPC* pPCAttacker = TO_PC(ch);
				CItem* weaponItem = pPCAttacker->m_wearInventory.wearItemInfo[WEARING_WEAPON];

				// 암흑 공격
				if (pPCAttacker->m_opAttackBlind > 0)
				{
					CSkill* pSkillBlind = gserver->m_skillProtoList.Create(415, pPCAttacker->m_opAttackBlind);
					int nRetApplySkill = 0;
					if (pSkillBlind)
					{
						bool bApply;
						nRetApplySkill = ApplySkill(ch, tch, pSkillBlind, -1, bApply);
					}
					delete pSkillBlind;
					pSkillBlind = NULL;
					if (nRetApplySkill != 0)
						return ;
				}

				// 독 공격
				if (pPCAttacker->m_opAttackPoison > 0)
				{
					CSkill* pSkillPoison = gserver->m_skillProtoList.Create(414, pPCAttacker->m_opAttackPoison);
					int nRetApplySkill = 0;
					if (pSkillPoison)
					{
						bool bApply;
						nRetApplySkill = ApplySkill(ch, tch, pSkillPoison, -1, bApply);
					}
					delete pSkillPoison;
					pSkillPoison = NULL;
					if (nRetApplySkill != 0)
						return ;
				}
			}
		} // PC 검사

		break;

	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
		// TODO : 소환수 공속 검사
		// 공격 거리 검사
		if (GetDistance(ch, tch) > ch->m_attackRange * 2)
			return ;

		// 공속 검사
		if (ch->CheckHackAttack(pc))
			return ;

		break;

	default:
		break;
	}

	if(IS_PC(tch))
	{
		if( TO_PC(tch)->m_bImmortal == true )
			return;
	}

	if (DEAD(ch) || !ch->CanAttack() || pc->IsDisable())	return ;
	// 결계걸린 대상은 공격 못한다.
	if ( tch->m_assist.m_state & AST_FREEZE )	return;
	//무적 버프 대상은 공격할 수 없다.
	if ( tch->m_assist.m_state & AST_SAFEGUARD )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_ATTACK_IMMOTAL);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}


	// 대상이 NPC일때만 멀티 공격
	if (!IS_NPC(tch))
		packet->multicount = 0;

	// 최소한 공격 1회 이상
	bool bAttacked = false;
	bool bAttackedPet = false;		// 애완동물은 NPC상대시 레벨 검사

	// 멀티 공격 검사용
	std::set<int> listMultiTarget;

	while (tch)
	{
		bool bBlocked = false;

		// NPC를 공격할 때에만 속성맵 검사 (프리PK 지역이 아닐때만)
		if ( IS_NPC(tch) && !(tch->GetMapAttr() & MATT_FREEPKZONE) )
		{
			char tempy = GET_YLAYER(ch);
			bBlocked = (!area->IsNotBlocked(ch, tch, true, tempy));
		}

		int ret = 0;
		if (!bBlocked)
		{
			if (IS_PC(ch) && ch->IsEnemy(tch))
			{
				bAttacked = true;
#ifdef MONSTER_AI
				if (tch != NULL	&& IS_NPC(tch))
				{
					CNPC * pTemp = TO_NPC(tch);
					if (pTemp != NULL)
					{
						if (ch->m_level - tch->m_level <= 5 && pTemp->m_proto->m_index != 303 /*악마의 묘지*/)
						{
							bAttackedPet = true;
						}

						if (pTemp->m_bMoveLock || pTemp->m_bMoveToRegen)
						{
							pTemp->m_bMoveToRegen = false;
							pTemp->m_bMoveLock = false;
							pTemp->m_pulseMoveLock = 0;
							pTemp->m_postregendelay = 0;
						}
					}
				}
#else
				if (tch != NULL	&& IS_NPC(tch) && ch->m_level - tch->m_level <= 5)
				{
					CNPC * pTemp = TO_NPC(tch);
					if (pTemp != NULL && pTemp->m_proto->m_index != 303 /*악마의 묘지*/)
					{
						bAttackedPet = true;
					}
				}
#endif
			}

			listMultiTarget.insert(tch->m_index);
			ret = ProcAttack(ch, tch, ch->GetAttackType(NULL), NULL, 0);
		}

		if (ret == -1 || tch->m_index == -1) // ProcAttack()안에서 Character 객체가 소멸된 경우도 포함
		{
			tch = NULL;
			continue ;
		}

		//공격 발동형 스킬 추가
		if( pc->m_optionAttSkillList.count() > 0 )
		{
			//공격 할 시에 적에게 스킬 적용
			void* pos = pc->m_optionAttSkillList.GetHeadPosition();
			bool bApply = false;
			while (pos)
			{
				CSkill* skill = pc->m_passiveSkillList.GetNext(pos);
				if (skill && skill->m_proto)
				{
					int rand = GetRandom(1, 10000);
					if( rand < skill->m_optionSkillProb )
					{
						ApplySkill(ch, tch, skill, -1, bApply);
						if(bApply == false)
						{
							GAMELOG << init("EVENT_PCBANG_2NDS SKILL FAILED (LOGIN) ", pc ) << end;// 스킬 적용 실패
						}
					}
				}
			}
		}

		if (area->m_zone->IsPersonalDungeon() == false)
		{
			tch = NULL;
			continue;
		}

		if (packet->multicount && !DEAD(ch))
		{
			int multitarget = packet->list[packet->multicount].index;
			--packet->multicount;

			tch = area->FindCharInCell(ch, multitarget, (MSG_CHAR_TYPE)packet->tCharType);
			if (tch == NULL)
			{
				continue;
			}

			if (GetDistance(ch, tch) > ch->m_attackRange * 2)
			{
				tch = NULL;
				continue;
			}

			std::set<int>::iterator it = listMultiTarget.find(tch->m_index);
			if (it != listMultiTarget.end())
			{
				GAMELOG << init("HACK ATTACK MULTI TARGET", pc)
						<< "ZONE" << delim
						<< pc->m_pZone->m_index << delim
						<< "TARGET" << delim
						<< packet->tCharType << delim
						<< multitarget
						<< end;

				if (pc->m_desc->IncreaseHackCount(1))
					return ;

				tch = NULL;
			}
		}
		else
		{
			tch = NULL;
			continue;
		}

		if (packet->multicount <= 0)
		{
			tch = NULL;
			continue;
		}
	} // end while

	if (bAttackedPet && !ch->IsInPeaceZone(true))
		pc->m_pulseLastAttackSkill = gserver->m_pulse;

#ifdef EVENT_SEARCHFRIEND_TIME
	// 공격시 이벤트 시간 갱신 검사
	if (gserver->m_bSearchFriendEvent && (pc->m_nEventSearchFriendListCount >= 1)
			&& (pc->m_bEventSearchFriendSelect == true) && (pc->m_nTimeEventSearchFriend <= 216000)
			&& bAttacked && !ch->IsInPeaceZone(true))
		pc->m_pulseEventSearchFriend = gserver->m_pulse;
#endif // #ifdef EVENT_SEARCHFRIEND_TIME
}
