#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_old_do_changejob.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"

bool do_ChangeJobReq(CPC* pc, CNetMsg::SP& msg);
bool do_ChangeJobResetReq(CPC* pc, CNetMsg::SP& msg);

void do_ChangeJob(CPC* pc, CNetMsg::SP& msg)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_CHANGEJOB_REQ:
		{
			if (do_ChangeJobReq(pc, msg) == false)
				return;
		}
		break;

	case MSG_CHANGEJOB_RESET_REQ:
		{
			if (do_ChangeJobResetReq(pc, msg) == false)
				return;
		}
		break;

	default:
		{
			LOG_ERROR("HACKING : invalid subtype[%d]. charIndex[%d]", pBase->subType, pc->m_index);
			pc->m_desc->Close("invalid subtype");
			return;
		}
	}

	// 소환 취소
	while (pc->m_elementalList)
		pc->UnsummonElemental(pc->m_elementalList);

	// 강신 취소
	if (pc->m_evocationIndex != EVOCATION_NONE)
		pc->Unevocation();
}

bool do_ChangeJobReq(CPC* pc, CNetMsg::SP& msg)
{
	if (pc->m_job2 != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_ALREADY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	if (pc->m_level < JOB_2ND_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_LEVEL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

#ifdef GIVE_ITEM_IN_CHANGE_JOB
	if(pc->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
#endif

	RequestClient::changeJob* packet = reinterpret_cast<RequestClient::changeJob*>(msg->m_buf);

	if (packet->npcIndex <= 0) // 전직 아이템을 쓴경우
	{
		if (pc->m_inventory.FindByDBIndex(2868) == NULL)
		{
			LOG_ERROR("HACKING : not found item(2868). charIndex[%d]", pc->m_index);
			pc->m_desc->Close("not found item");
			return false;
		}
	}
	else
	{
		if (pc->m_pArea->FindCharInCell(pc, packet->npcIndex, MSG_CHAR_NPC, false) == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOT_EXIST_NPC);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
	}

	CSkill* skill = NULL;
	int needskilllevel = 5;

	// 스킬 구하기 : 필요 스킬 레벨이 5가 아니면 수정 필요
	switch (pc->m_job)
	{
	case JOB_TITAN:
		{
			switch (packet->job)
			{
			case JOB_2ND_HIGHLANDER:
			case JOB_2ND_WARMASTER:
				skill = pc->m_activeSkillList.Find(43);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

	case JOB_KNIGHT:
		{
			switch (packet->job)
			{
			case JOB_2ND_ROYALKNIGHT:
			case JOB_2ND_TEMPLEKNIGHT:
				skill = pc->m_activeSkillList.Find(48);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

	case JOB_HEALER:
		{
			switch (packet->job)
			{
			case JOB_2ND_ARCHER:
			case JOB_2ND_CLERIC:
				skill = pc->m_activeSkillList.Find(52);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

	case JOB_MAGE:
		{
			switch (packet->job)
			{
			case JOB_2ND_WIZARD:
			case JOB_2ND_WITCH:
				skill = pc->m_activeSkillList.Find(65);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

	case JOB_ROGUE:
		{
			switch (packet->job)
			{
			case JOB_2ND_ASSASSIN:
			case JOB_2ND_RANGER:
				skill = pc->m_activeSkillList.Find(131);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

	case JOB_SORCERER:
		{
			switch (packet->job)
			{
			case JOB_2ND_ELEMENTALLIST:
			case JOB_2ND_SPECIALLIST:
				skill = pc->m_activeSkillList.Find(308);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;

#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
		{
			switch (packet->job)
			{
			case JOB_2ND_EX_ASSASSIN:
			case JOB_2ND_EX_RANGER:
				// EXROGUE : 전직 시, 반드시 배워야할 스킬 인덱스 수정
				skill = pc->m_activeSkillList.Find(1536);
				break;
			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				break;
			}
		}
		break;
#endif // EX_ROGUE

#ifdef EX_MAGE
	case JOB_EX_MAGE:
		{
			switch (packet->job)
			{
			case JOB_2ND_WIZARD:
			case JOB_2ND_WITCH:
				// EXMAGE : 전직 시, 반드시 배워야할 스킬 인덱스 수정
				skill = pc->m_activeSkillList.Find(1638);
				break;

			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
					SEND_Q(rmsg, pc->m_desc);
				}
				return false;
			}
		}
		break;
#endif // EX_MAGE

	default:
		{
			LOG_ERROR("HACKING : invalid job[%d]. charIndex[%d]", packet->job, pc->m_index);
			pc->m_desc->Close("invalid job");
			return false;
		}
	}

	// --- 스킬 구하기
	if (skill == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	if (skill->m_level < needskilllevel)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	int needmoney = 50000;

	if (pc->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOMONEY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	else
	{
		pc->m_inventory.decreaseMoney(needmoney);
	}

	GAMELOG << init("CHANGE JOB", pc)
			<< "2ND JOB" << delim
			<< packet->job << delim
			<< "ORIGINAL STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << delim;

	pc->m_job2 = packet->job;

	// 스탯 재계산
	// 스탯 포인트 초기화
	pc->m_statpt_str = 0;
	pc->m_statpt_dex = 0;
	pc->m_statpt_int = 0;
	pc->m_statpt_con = 0;
	pc->m_statpt_remain = (pc->m_level - JOB_2ND_LEVEL) * 3 + (pc->m_level - JOB_2ND_LEVEL) / 2 + (JOB_2ND_LEVEL - 1);
	if ( pc->m_bPhoenix_Char == 1 )
	{
		// 피닉스로 전직하면서 추가된 30 스텟 포인트 재적립
		pc->m_statpt_remain += 30;
	}

	GAMELOG << "RESET STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << end;

	pc->InitStat();
	pc->CalcStatus(true);

	pc->m_hp = pc->m_maxHP;
	pc->m_mp = pc->m_maxMP;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJob(rmsg, pc->m_index, pc->m_job, pc->m_job2);
		pc->m_pArea->SendToCell(rmsg, pc, true);
	}

	if (pc->IsParty())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMemberChangeJobMsg(rmsg, pc->m_party->GetBossIndex(), pc->m_index, pc->m_job, pc->m_job2);
		SEND_Q(rmsg, gserver->m_helper);
	}

#ifdef GIVE_ITEM_IN_CHANGE_JOB
	// SP지급
	// pc->AddExpSP(0, 10000000, false, false, true); 크랜이 나스로 직업포기를 해주어서 숙련도 무한 작업이 가능하여 뺌.
	// 아이템 지급
	//0:타이탄, 1:나이트, 2:힐러, 3:메이지, 4:로그, 5:소서러, 6:나이트쉐도우(dummy), 7:EX 로그, 8:EX 메이지
#ifdef EX_ROGUE
#ifdef EX_MAGE
	const int nGiveItem[JOBCOUNT] = {6893, 6895, 6897, 6899, 6901, 6903, 0, 6901, 6899};
#else
	const int nGiveItem[JOBCOUNT] = {6893, 6895, 6897, 6899, 6901, 6903, 0, 6901};
#endif // EX_MAGE
#else
	const int nGiveItem[6] = {6893, 6895, 6897, 6899, 6901, 6903};
#endif // EX_ROGUE

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nGiveItem[pc->m_job], -1, 0, 0, 1);
	if(pItem == NULL)
	{
		GAMELOG << init("ITEM ERROR") << "ITEM IS NOT EXIST" << "ITEM INDEX" << delim << nGiveItem[pc->m_job] << end;
		return true; // 아이템을 지급하지 못한뿐 전직은 성공
	}

	if (pc->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return true; // 아이템을 지급하지 못한뿐 전직은 성공
	}
#endif

	return true;
}

bool do_ChangeJobResetReq(CPC* pc, CNetMsg::SP& msg)
{
	RequestClient::changeJobReset* packet = reinterpret_cast<RequestClient::changeJobReset*>(msg->m_buf);

	CNPC* pNpc = (CNPC*)pc->m_pArea->FindCharInCell(pc, packet->npcIndex, MSG_CHAR_NPC, false);
	if (pNpc == NULL || pNpc->IsFlag(NPC_RESETSTAT) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	if (pc->m_job2 == 0	|| pc->m_job == JOB_NIGHTSHADOW)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	LONGLONG statptall = pc->m_statpt_str + pc->m_statpt_dex + pc->m_statpt_int + pc->m_statpt_con;
	LONGLONG needNas = statptall * (LONGLONG)(25000 * pc->m_level / JOB_2ND_LEVEL) + 100000;

	if (pc->m_inventory.getMoney() < needNas)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobError(rmsg, MSG_CHANGEJOB_ERROR_NOMONEY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	pc->m_inventory.decreaseMoney(needNas);

	GAMELOG << init("CHANGE JOB", pc)
			<< "2ND JOB" << delim
			<< pc->m_job2 << delim
			<< "NEED MONEY" << delim
			<< needNas << delim
			<< "ORIGINAL STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << end;

	// 초기화
	// 직업
	pc->m_job2 = 0;

	// 스킬
	int skillpoint = 0;
	skillpoint += pc->m_activeSkillList.RemoveSkillFor2ndJob();
	skillpoint += pc->m_passiveSkillList.RemoveSkillFor2ndJob();
	skillpoint += pc->m_etcSkillList.RemoveSkillFor2ndJob();

	// 스탯 포인트
	pc->m_statpt_str = 0;
	pc->m_statpt_dex = 0;
	pc->m_statpt_int = 0;
	pc->m_statpt_con = 0;
	pc->m_statpt_remain = pc->m_level - 1;

	pc->InitStat();

	pc->CalcStatus(true);

	pc->m_hp = pc->m_maxHP;
	pc->m_mp = pc->m_maxMP;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobReset(rmsg, pc);
		pc->m_pArea->SendToCell(rmsg, pc, true);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}

	if (pc->IsParty())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMemberChangeJobMsg(rmsg, pc->m_party->GetBossIndex(), pc->m_index, pc->m_job, pc->m_job2);
		SEND_Q(rmsg, gserver->m_helper);
	}

	return true;
}
