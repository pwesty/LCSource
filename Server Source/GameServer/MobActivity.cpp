#include <boost/scoped_array.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Area.h"
#include "CmdMsg.h"
#include "Zone.h"
#include "Server.h"
#include "Env.h"
#include "../ShareLib/DBCmd.h"
#include "Battle.h"
#include "hardcoding.h"
#include "DratanCastle.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_old_do_move.h"
#include "ExtremeCube.h"

// npc 회복
void MobRecover(CNPC* npc);

// npc 스킬 사용, 이후 npc ai 스킵이 필요하면 true
bool MobProcSkill(CNPC* npc);

//bool MobProcSkillMercenary(CNPC *npc);
bool MobProcSkillTotem_Buff(CNPC *npc);
bool MobProcSkillTrap_Attk(CNPC *npc);
bool MobProcSkillSuicide_Attk(CNPC *npc);
bool MobProcSkillItemTotem_Buff(CNPC *npc);
int GetSummonNpcMultiTarget(CNPC* npc, MultiTarget *multitarget, int mcount);

// npc 피어 처리, 피어로 도망가는 npc이면 true
bool MobProcFear(CNPC* npc, CCharacter* target);

////////////////////
// Function name	: MobMoveTo
// Description	    : npc를 x, z로 이동
// Return type		: float
//                  : 대상지점과의 남은 거리
// Argument         : CNPC* npc
//                  : 이동할 NPC
// Argument         : float x
// Argument         : float z
//                  : 이동할 위치
// Argument         : bool bRun
//                  : 뛰어서 이동할지 여부
// Argument         : bool bAttack
//                  : 공격을 위한 이동인지 여부 (x, z 좌표의 속성맵을 검사할지 결정하는데 쓰임)
// Argument         : CCharacter* target
//                  : target은 이동에서 블럭되지 않음
float MobMoveTo(CNPC* npc, float x, float z, bool bRun, bool bAttack, CCharacter* target);

// npc의 공격 대상 선정
CCharacter* MobSelectTarget(CNPC* npc);

CCharacter* MobSelectTarget_Taming_or_Confused(CNPC* npc);

// npc 랜덤 이동
void MobMoveRandom(CNPC* npc);

// npc 스킬 선택
CCharacter* MobSelectSkill(CNPC* npc, CCharacter* target);

// npc 스킬 레디, 레디 했으면 true
bool MobReadySkill(CNPC* npc, CCharacter* target);

/////////////////////////////////////////////////////////////////
// 몹 행동 루틴
void CArea::MobActivity()
{
	CNPC* npc = NULL;
	CNPC* tnpc = m_npcList;

	CCharacter* target = NULL;

	while ((npc = tnpc) && npc->m_proto != NULL)
	{
		tnpc = npc->m_pNPCNext;

		if(tnpc != NULL && this->m_zone->m_index != tnpc->m_pZone->m_index)
			return;

		if( npc->Check_MobFlag(STATE_MONSTER_MERCENARY)
				|| npc->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF)
				|| npc->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK)
				|| npc->Check_MobFlag(STATE_MONSTER_TRAP)
				|| npc->Check_MobFlag(STATE_MONSTER_SUICIDE) )
		{
			// 이넘들은 바로삭제가 아닌
			if( npc->m_hp == 0 )
				continue;

			// 오너가 없으면 무조건 삭제
			if( !npc->GetOwner() )
			{
				npc->m_hp = 0;
				npc->m_pArea->m_nNPC_Killed_NPC ++;
			}
		}

		if( npc->m_hp == 0 )
			continue;

		npc = MobTimeCheck(npc);

		target = NULL;

		if (npc == NULL )
			continue ;

		if(npc->m_proto->CheckFlag(NPC_CASTLE_GUARD)
				&& npc->m_regenInfo
				&& m_zone->m_index == ZONE_DRATAN)
		{
			// 드라탄 수호병인 경우 공성전 시간이 아닐 때  움직이지 않음
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if( pCastle->GetState() == WCSF_NORMAL)
			{
				continue;
			}
		}

#ifdef SYSTEM_TREASURE_MAP
		if( npc->m_idNum == TREASURE_BOX_NPC_INDEX
				&& npc->m_regenTime + TREASURE_BOX_NPC_REGEN < gserver->getNowSecond() )
		{
			//npc->m_pZone->RemoveTreasureBoxNpc(npc);
			npc->SendDisappearAllInCell( false );
			DelAttackList( npc );
			CharFromCell( npc, true );
			DelNPC( npc );
			npc = NULL;
			continue;
		}
#endif

		if( npc->m_proto->m_index == 314 )
		{
			if( npc->m_regenTime + PANDOR_REGEN_TIME <= gserver->m_pulse )
			{
				if(npc->m_regenInfo)
				{
					npc->m_regenInfo->m_lastDieTime = gserver->m_pulse - npc->m_regenTime;
					npc->SendDisappearAllInCell( false );

					DelAttackList( npc );

					CharFromCell( npc, true );
					DelNPC( npc );
					npc = NULL;
					continue;
				}
			}
		}

		if (npc->m_proto->CheckFlag(NPC_RAID) && npc->m_regenInfo)
		{
			if (npc->m_nRaidMoveTime <= gserver->m_pulse)
			{
				CNPCRegenInfo* pRegeninfo = npc->m_regenInfo;
				npc->SendDisappearAllInCell(false);
				DelAttackList(npc);
				CharFromCell(npc, true);
				DelNPC(npc);
				pRegeninfo->m_lastDieTime = gserver->m_pulse - pRegeninfo->m_regenSec;
				pRegeninfo->m_bRaidMoveRandom = true;
				npc = NULL;
				continue ;
			}
		}

		// NPC 회복
		MobRecover(npc);

		if (gserver->m_pulse - npc->m_aipulse < npc->m_delay)
			continue ;

		static const int		nIndexWarrior		= 310;					// 전사의 손길
		static const int		nIndexGuard			= 311;					// 수호의 손길
		static const int		nIndexBrave			= 312;					// 용맹의 손길
		static const int		nIndexProtect		= 313;					// 보호의 손길

		switch (npc->m_idNum)
		{
		case nIndexWarrior:
		case nIndexGuard:
		case nIndexBrave:
		case nIndexProtect:
			DropBlessWarrior(npc);
			npc = NULL;
			break;
		default:
			break;
		}

		if (npc == NULL)
			continue ;

		// 상점, 존이동, 연금술사, 퀘스트, 스킬마스터, 특수스킬마스터, 면죄부, 이벤트 NPC, 권좌 무시
		if (npc->m_proto->CheckFlag(NPC_SHOPPER | NPC_ZONEMOVER | NPC_REFINER | NPC_QUEST | NPC_REMISSION | NPC_EVENT | NPC_LORD_SYMBOL))
			continue ;

		// 스킬 처리
		if( npc->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF) )
		{
			if( MobProcSkillTotem_Buff(npc) )
				continue;
		}
		else if( npc->Check_MobFlag(STATE_MONSTER_TRAP) )
		{
			MobProcSkillTrap_Attk(npc);
			continue;
		}
		else if( npc->Check_MobFlag(STATE_MONSTER_SUICIDE) )
		{
			if( MobProcSkillSuicide_Attk(npc) )
				continue;
		}
		else if( npc->Check_MobFlag(STATE_MONSTER_TOTEM_ITEM_BUFF) )
		{
			if( MobProcSkillTotem_Buff(npc) )
				continue;
		}
		else if (MobProcSkill(npc))
			continue;

		// 스킬 처리 이후에 스킬이 SKILL_CON_NORMAL이면 스킬 대상을 향해 이동하도록 함
		if (npc->m_currentSkill && npc->m_currentSkill->m_state == SKILL_CON_NORMAL)
		{
			// 스킬의 타겟을 찾는다
			target = npc->m_pArea->FindCharInCell(npc, npc->m_currentSkill->m_targetIndex, npc->m_currentSkill->m_targetType, false);
			if ( target == NULL || target == npc || (!npc->CanMove() && GetDistance((CCharacter*)npc, target) > npc->m_currentSkill->m_proto->GetFireRange(npc) + 1.0f) )
			{
				npc->m_currentSkill->m_usetime = 0;
				npc->m_currentSkill = NULL;
			}
		}

		// 1초
		npc->m_aipulse = gserver->m_pulse;
		npc->m_delay = NPC_ACTIVITY_DELAY;

		// 독립적으로 처리
		if ( npc->Check_MobFlag( STATE_MONSTER_TAMING ) )
		{
			// npc가 테이밍에 걸리지 않았으면 몹플레그도 해제시킨다.
//			if(npc->m_assist.FindByType(MT_ASSIST, MST_ASSIST_TAMING) <= 0)
//				npc->reSet_MobFlag(STATE_MONSTER_TAMING);
//			else
			MobActivity_Taming(npc);
			continue;
		}

		else if ( npc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
		{
			// npc가 혼돈에 걸리지 않았으면 몹플레그도 해제시킨다.
			if(npc->m_assist.FindByType(MT_ASSIST, MST_ASSIST_CONFUSED) <= 0)
				npc->reSet_MobFlag(STATE_MONSTER_CONFUSION);
			else
				MobActivity_Confused(npc);
			continue;
		}


#ifndef BUGFIX_MERCENARY_GETOWNER_NULL_SERVERDOWN // 아래에도 처리하는 루틴이 있음 삭제하는 것이 좋을듯.
		if ( npc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
		{
			MobActivity_Mercenary(npc);
			continue;
		}
#endif

		if( npc->GetOwner() )
		{
			switch( npc->GetOwner()->GetSummonNpcType(npc) )
			{
			case SUMMON_NPC_TYPE_MERCENARY:
				MobActivity_Mercenary(npc);
				continue;
			case SUMMON_NPC_TYPE_TOTEM:
				MobActivity_Totem(npc);
				continue;
			case SUMMON_NPC_TYPE_BOMB_1:
			case SUMMON_NPC_TYPE_BOMB_2:
			case SUMMON_NPC_TYPE_BOMB_3:
				MobActivity_Suicide(npc);
				continue;
			case SUMMON_NPC_TYPE_TOTEM_ITEM:
				MobActivity_Item_Totem(npc);
				continue;
			}
		}
		// 피어 처리
		if (MobProcFear(npc, NULL))
			continue ;

		// 타겟 선정
		if (!target)
		{
			target = MobSelectTarget(npc);

			// 탱커 타입 플래그 2이면 HP가 20%미만일때 도망
			// 데미지 딜러 타입 플래그 3이면 HP가 20% 미만일때 도망
			if ( (npc->m_proto->m_aitype == NPC_AI_TANKER && npc->m_proto->CheckAIBit(2) && npc->m_hp < npc->m_maxHP * 20 / 100) ||
					(npc->m_proto->m_aitype == NPC_AI_DAMAGEDEALER && npc->m_proto->CheckAIBit(3) && npc->m_hp < npc->m_maxHP * 20 / 100) )
			{
				MobProcFear(npc, target);
				continue ;
			}

			// 몹 스킬 선택
			target = MobSelectSkill(npc, target);
		}
#ifdef MONSTER_AI
		if (!target && npc->m_bMoveToRegen)
		{
			if (npc->GetDistToRegen() <= 0.3f)
			{
				npc->m_bMoveToRegen = false;
				npc->m_bMoveLock = true;
				npc->m_postregendelay = gserver->m_pulse;
			}
			else
			{
				MobMoveTo(npc, npc->m_regenX, npc->m_regenZ, false, false, NULL);
			}

			continue;
		}

		if (npc->m_bMoveLock)
		{
			if (npc->m_postregendelay)
				continue;

			// 타겟이 없거나, 타겟이 있는데, 1분동안 무적 상태였다면 사라진다.
			if (!target || (target && npc->m_pulseMoveLock != 0 && gserver->m_pulse >= npc->m_pulseMoveLock + PULSE_REAL_MIN))
			{
				npc->m_bMoveLock = false;
				npc->m_pulseMoveLock = 0;

				npc->m_hp = npc->m_maxHP;
				npc->m_mp = npc->m_maxMP;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					CharStatusMsg(rmsg, npc, 0);
					npc->m_pArea->SendToCell(rmsg, npc, false);
				}

				npc->m_nBlockRegen = npc->m_index;
				MobMoveTo(npc, npc->m_regenX, npc->m_regenZ, false, false, NULL);

				continue;
			}
		}
#endif

		// 타겟이 없으면 랜덤 이동
		if (!target)
		{
			if ( !this->IsFreeze(gserver->m_pulse) )
				MobMoveRandom(npc);

			continue ;
		}

#ifdef RESTRICT_PVP_SKILL
		if (target && IS_PC(target))
		{
			CPC* pTargetPC = TO_PC(target);
			pTargetPC->m_nRestrictPvPSkillDelaySec = RESTRICT_PVP_SKILL;
		}
#endif // RESTRICT_PVP_SKILL

		// NPC가 공격을 해도 전투 상태를 저장
		if (target && !IS_NPC(target))
			npc->m_nRaidMoveTime = gserver->m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);

		if ( target && IS_NPC(target) )
		{
			CNPC* pNpc = TO_NPC(target);

			if(!pNpc)
				continue;

			if( target->m_index != npc->m_index )
			{
#ifdef ATTACK_LIST_BUG_FIX
				// 일반 몬스터가 혼란 몬스터를 때리는 현상 수정
				if ( pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
				{
					DelAttackList(target);
					continue ;
				}
#else //ATTACK_LIST_BUG_FIX
				// 일반 몬스터가 다른 일반 몬스터를 따라가는 현상 수정
				// 어택리스트에 추가가 되나 공격은 못하므로 따라만 다니게 된 것이었음
				if( !pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) && !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
				{
					DelAttackList(target);
					continue ;
				}

				if( !pNpc->Check_MobFlag( STATE_MONSTER_MERCENARY ) && !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
				{
					DelAttackList(target);
					continue ;
				}

				// 일반 몬스터가 혼란 몬스터를 때리는 현상 수정
				else if ( pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
				{
					DelAttackList(target);
					continue ;
				}

				// 일반 몬스터가 혼란 몬스터를 때리는 현상 수정
				else if ( pNpc->Check_MobFlag( STATE_MONSTER_TOTEM_BUFF ) )
				{
					DelAttackList(target);
					continue ;
				}

				// 일반 몬스터가 혼란 몬스터를 때리는 현상 수정
				else if ( pNpc->Check_MobFlag( STATE_MONSTER_TOTEM_ATTK ) )
				{
					DelAttackList(target);

					continue ;
				}
#endif // ATTACK_LIST_BUG_FIX
			}
		}

		// 타겟이 있으면 타겟을 향해 이동
		if(target->m_pArea->m_zone->isRVRZone() &&
				target->getSyndicateType() == npc->getSyndicateType())
		{
			DelAttackList(target);
			continue;
		}

		if(IS_PC(target))
		{
			CPC* pc = TO_PC(target);
			if(pc->m_bVisible == false)
				continue;
		}

		float dist = MobMoveTo(npc, GET_X(target), GET_Z(target), true, true, target);
#if defined (LC_GAMIGO)
		char outY;
#endif // defined (LC_GAMIGO)

		if (dist <= 0.3f && npc->m_proto->CheckFlag(NPC_ATTACK) && npc->CanAttack()
#if defined (LC_GAMIGO)
				&& npc->m_pArea && npc->m_pArea->IsNotBlocked(npc, target, true, outY)
#endif // defined (LC_GAMIGO)
		   )
		{
			// 공격 처리
			// 어택했을경우 AI관련변수 초기화
			npc->InitAIVar();

#ifdef MONSTER_AI
			npc->m_bMoveLock = false;
			npc->m_pulseMoveLock = 0;
#endif

			if (!npc->m_bStop)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeMoveMsg(rmsg, MSG_MOVE_STOP, npc);
				npc->m_bStop = true;
				SendToCell(rmsg, npc, false);
			}
			if (target)
			{
				// 수호탑은 스킬만 사용가능
				if (!(npc->m_currentSkill == NULL && (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))))
				{
					if (!MobReadySkill(npc, target) && gserver->m_pulse - npc->m_pulseAttack >= npc->m_attackSpeed * 15 / 10 && npc->IsEnemy(target))
					{
						npc->m_pulseAttack = gserver->m_pulse;
						ProcAttack( (CCharacter *)npc, target, npc->GetAttackType(NULL), NULL, 0);
					}
				}
			}
		}
#ifdef MONSTER_AI
		// 타겟은 있지만, 공격이 안될 경우
		else
		{
			if (npc->m_bMoveLock)
				continue;

			// 몬스터 위치가 변하지 않으면
			if (npc->m_nOldDist == dist)
			{
				npc->m_bMoveLock = true;
				npc->m_pulseMoveLock = gserver->m_pulse;
			}
			else
			{
				// 리젠 위치와 100M 이상 or 타겟과의 거리가 50M 이상
				if (GetDistance((CCharacter*)npc, target) > 50.0f || npc->GetDistToRegen() > 100.0f)
				{
					npc->m_hp = npc->m_maxHP;
					npc->m_mp = npc->m_maxMP;
					//npc->m_walkSpeed += 5;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						CharStatusMsg(rmsg, npc, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					npc->m_bMoveToRegen = true;
					DelTargetFromAttackList(npc, target);
				}
			}
		}

		npc->m_nOldDist = dist;
#endif
	}
}

CNPC* CArea::MobTimeCheck(CNPC* npc)
{
	if (!npc)
		return NULL;

	if (DEAD(npc))
		return NULL;

	// TODO : 몹 상태에 관련된 시간 검사

	npc->m_assist.DecreaseTime();

	// 몬스터가 영혼을 빼앗긴 뒤 일정 시간이 지나면 영혼을 다시 재생한다.
	if (npc->m_pulseSoulRecover != 0 && npc->m_pulseSoulRecover + PULSE_REAL_MIN < gserver->m_pulse)
	{
		npc->m_pulseSoulRecover = 0;
		npc->reSet_MobFlag(STATE_MONSTER_NO_SOUL);
	}

	// 보이면 return
	if (npc->m_disableTime <= 0)
	{
		// 050310 : bs : 리젠이후 일정시간 멈춤
		if (!npc->m_postregendelay)
		{
			return npc;
		}
		else if (gserver->m_pulse >= npc->m_postregendelay + PULSE_POST_REGEN_DELAY)
		{
			npc->m_postregendelay = 0;

#ifdef MONSTER_AI
			npc->m_bMoveLock = false;
#endif

			return npc;
		}
		// --- 050310 : bs : 리젠이후 일정시간 멈춤
	}

	// 리젠 후 딜레이가 풀리면 리젠 메시지를 보내고
	else if (npc->m_disableTime <= time(0))
	{
		npc->m_disableTime = 0;
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, npc, true);
			SendToCell(rmsg, npc);
		}

		// 050310 : bs : 리젠이후 일정시간 멈춤 : 선공 몬스터만
		if (npc->m_proto->CheckFlag(NPC_FIRSTATTACK))
			npc->m_postregendelay = gserver->m_pulse;
		else
			npc->m_postregendelay = 0;
		// --- 050310 : bs : 리젠이후 일정시간 멈춤

		return NULL;
	}

	return NULL;
}

void CArea::AddNPC(CNPC* npc)
{
	if (npc->m_idNum < 0)
		return;

	ADD_TO_BILIST(npc, m_npcList, m_pNPCPrev, m_pNPCNext);
	npc->m_pZone = m_zone;
	npc->m_pArea = this;
	m_nCountNPC++;

	if (npc->m_lifetime > 0)
	{
		CArea::m_lifetime_set.insert(npc);
	}
}

void CArea::DelNPC(CNPC* npc)
{
	if (npc->m_pArea != this)
		return ;
	REMOVE_FROM_BILIST(npc, m_npcList, m_pNPCPrev, m_pNPCNext);

	if (npc->m_regenInfo)
	{
		npc->m_regenInfo->m_lastDieTime = gserver->m_pulse;
		npc->m_regenInfo->m_bAlive = false;
	}

#ifdef SYSTEM_TREASURE_MAP
	if( npc->m_pZone && npc->m_idNum == TREASURE_BOX_NPC_INDEX )
		npc->m_pZone->RemoveTreasureBoxNpc(npc);
#endif
	if (npc->m_lifetime > 0)
	{
		CArea::m_lifetime_set.erase(npc);
	}

	npc->m_pZone = NULL;
	npc->m_pArea = NULL;
	npc->DeleteNPC();
	npc = NULL;

	m_nCountNPC--;
}

void CArea::MobRegen()
{
	bool bRegenRaid = false;

	// npc RegenList 중에서
	for (int i=0; i < m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = m_npcRegenList.m_infoList + i;

		// npc RegenInfo 중에 살아날 시간이 되고
		if (p && p->m_bRegen && p->m_lastDieTime != 0)
		{
			int cx, cz;
			int nRegenSecAdjust = p->m_regenSec;

#ifdef TLD_EVENT_SONG
			switch (p->m_npcIdx)
			{
			case 1622:
			case 1623:
				if (gserver->tld_event == false)
				{
					continue;
				}
				break;
			}
#endif

			// 보스몹은 제외
			CNPCProto* pNPCProto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
			if (!pNPCProto)
				continue ;

			if(m_zone->m_index == ZONE_DRATAN && pNPCProto->CheckFlag(NPC_CASTLE_GUARD))
			{
				CDratanCastle * pCastle = CDratanCastle::CreateInstance();
				if(pCastle->GetState() == WCSF_NORMAL)
				{
					// 공성전이 아니면
					continue;
				}
			}

			if (pNPCProto->CheckFlag(NPC_BOSS | NPC_MBOSS | NPC_RAID) == 0
					&& !(m_zone->m_index == ZONE_DRATAN && pNPCProto->CheckFlag(NPC_CASTLE_GUARD)))	// 드라탄 수호병 제외
			{
				// 리젠 되는 셀을 중심으로 3x3을 검사하여 리젠시간을 조정한다. ( 해당 셀의 리젠이 있을 경우만 )
				PointToCellNum(p->m_regenX, p->m_regenZ, &cx, &cz);
				if (m_cell[cx][cz].m_nTotalRegen > 0)
				{
					float fAdjust = 1.0f;
					int nCountNPC1x1 = 0;
					int nCountNPC3x3 = 0;
					int nCountNPCRegen1x1 = 0;
					int nCountNPCRegen3x3 = 0;
					int nLoopCellX;
					int nLoopCellZ;
					for (nLoopCellX = cx - 1; nLoopCellX <= cx + 1; nLoopCellX++)
					{
						if (nLoopCellX < 0 || nLoopCellX >= m_size[0])
							continue ;
						for (nLoopCellZ = cz - 1; nLoopCellZ <= cz + 1; nLoopCellZ++)
						{
							if (nLoopCellZ < 0 || nLoopCellZ >= m_size[1])
								continue ;
							if (cx == nLoopCellX && cz == nLoopCellZ)
							{
								nCountNPC1x1 += m_cell[nLoopCellX][nLoopCellZ].m_nCountNPC;
								nCountNPCRegen1x1 += m_cell[nLoopCellX][nLoopCellZ].m_nTotalRegen;
							}
							nCountNPC3x3 += m_cell[nLoopCellX][nLoopCellZ].m_nCountNPC;
							nCountNPCRegen3x3 += m_cell[nLoopCellX][nLoopCellZ].m_nTotalRegen;
						}
					}
					if (nCountNPCRegen3x3 > 0 && nCountNPCRegen3x3 / 2 >= nCountNPC3x3)
						fAdjust -= fAdjust / 3;
					if (nCountNPCRegen1x1 > 0 && nCountNPCRegen1x1 / 2 >= nCountNPC1x1)
						fAdjust -= fAdjust / 2;
					nRegenSecAdjust = (int)(nRegenSecAdjust * fAdjust);

					// 최소 NPC 리젠은 10초
					if (p->m_regenSec > 100 && nRegenSecAdjust < 100)
						nRegenSecAdjust = 100;
				}
			}
#ifdef TLD_EVENT_SONG
			if(p->m_npcIdx == 1622)
			{
				nRegenSecAdjust = p->m_regenSec;
			}
#endif
			if (gserver->m_pulse - p->m_lastDieTime < nRegenSecAdjust)
				continue ;

			// totalnum이 -1이면 무한리젠
			// totalnum이 0이면 스킵
			if (p->m_totalNum == 0)
				continue ;
			//  totalNum이 -1 아니고 numRegen이 totalNum 보다 크면 NPC가 있으면 리젠안함
			if ((p->m_totalNum != -1) && (p->m_numRegen >= p->m_totalNum))
				continue;

			CNPC* npc;
			npc = gserver->m_npcProtoList.Create(p->m_npcIdx, p);
			if(!npc)
			{
				p->m_totalNum = 0;
				continue;
			}

			// 속성맵상 피스존이고 피스 NPC가 아니면 리젠안함
			if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
			{
				unsigned short mapAttr = GetAttr(p->m_regenY, p->m_regenX, p->m_regenZ);
				if(mapAttr & MATT_UNWALKABLE || mapAttr & MATT_PEACE )
				{
					GAMELOG << init("REGEN error")
						<< "NPC" << delim
						<< npc->m_name << delim
						<< p->m_npcIdx << delim
						<< "ZONE" << delim
						<< m_zone->m_index << delim
						<< "YLAYER" << delim
						<< p->m_regenY << delim
						<< "X" << delim
						<< p->m_regenX << delim
						<< "Z" << delim
						<< p->m_regenZ
						<< end;
					delete npc;
					p->m_totalNum = 0;
					continue ;
				}
			}

			GET_X(npc) = p->m_regenX;
			GET_YLAYER(npc) = p->m_regenY;
			GET_Z(npc) = p->m_regenZ;

			// 어택 가능한 NPC
			if (npc->m_proto->CheckFlag(NPC_ATTACK) && npc->m_proto->m_index != 339)
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			else
				GET_R(npc) = p->m_regenR;

			if (!bRegenRaid && p->m_npcIdx != 314 && npc->m_proto->CheckFlag(NPC_ATTACK) && RaidMobRegen(GET_X(npc), GET_Z(npc), GET_R(npc), GET_YLAYER(npc)))
			{
				delete npc;
				npc = NULL;
				p->m_lastDieTime = gserver->m_pulse;
				bRegenRaid = true;
				continue ;
			}
			bRegenRaid = true;

			if( p->m_npcIdx == 314 )
			{
				int	RandomNPCIndex = 0;
				int nTry = 0;

//				m_npcRegenList.m_infoList[RandomNPCIndex];
				CNPCProto* pTempProto = NULL;
				while (pTempProto == NULL)
				{
					RandomNPCIndex = GetRandom( 0, m_npcRegenList.m_nCount - 5 );
					pTempProto = gserver->m_npcProtoList.FindProto(m_npcRegenList.m_infoList[RandomNPCIndex].m_npcIdx);
					if ( pTempProto && ( pTempProto->CheckFlag( NPC_PEACEFUL ) || pTempProto->m_index == 314 ) )
						pTempProto = NULL;
				}

				do
				{
					GET_X(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenX + ( GetRandom( 0, 1 ) ? -1 : 1 ) * GetRandom( 20, 50 ) / 10.0f;
					GET_Z(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenZ + ( GetRandom( 0, 1 ) ? -1 : 1 ) * GetRandom( 20, 50 ) / 10.0f;
					GET_YLAYER(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenY;
					GET_R(npc) = GetRandom( 0, (int)(PI_2 * 10000 ) ) / 10000;
					if ( GetAttr( GET_YLAYER(npc), GET_X(npc), GET_Z(npc) ) & MATT_WALKABLE )
						break;
					nTry++;
				}
				while (nTry <= 10 );
				if( nTry > 10 )
				{
					GET_X(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenX;
					GET_Z(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenX;
					GET_YLAYER(npc) = m_npcRegenList.m_infoList[RandomNPCIndex].m_regenY;
					GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				}
			}

			npc->m_regenTime	= gserver->m_pulse;

			npc->m_regenX = GET_X(npc);
			npc->m_regenY = GET_YLAYER(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_numRegen++;

			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
//			npc->m_disableTime = NPC_DISABLE_TIME;
			npc->m_disableTime = time(0) + 3;

			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if(m_zone->m_index == ZONE_DRATAN_CASTLE_DUNGEON && !npc->m_proto->CheckFlag(NPC_ZONEMOVER))
			{
				static const int nMobRate[10][4] =  	// 공격력, 방어력, 최대체력, 시야범위
				{
					{20, 20, 30, 0},
					{10, 10, 15, 0},
					{5, 5, 10, 0},
					{0, 0, 0, 0},
					{-5, -5, -5, 0},
					{-7, -7, -7, 0},
					{-10, -10, -10, 0},
					{-15, -15, -15, 0},
					{-20, -20, -20, 0},
					{-25, -25, -25, -80},
				};

				int midx = pCastle->m_dvd.GetMobRate()/10;
				if(midx > 9)
				{
					midx = 9;
				}

				// 공격력 적용
				if(npc->m_attackType == MSG_DAMAGE_MELEE)
				{
					npc->m_eqMelee = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
					if(npc->m_eqMelee < 0)
					{
						npc->m_eqMelee = 1;
					}
				}
				else if(npc->m_attackType == MSG_DAMAGE_RANGE)
				{
					npc->m_eqRange = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
					if(npc->m_eqRange < 0)
					{
						npc->m_eqRange = 1;
					}
				}
				else if(npc->m_attackType == MSG_DAMAGE_MAGIC)
				{
					npc->m_eqMagic = npc->m_proto->m_magic + npc->m_proto->m_magic * nMobRate[midx][0] / 100;
					if(npc->m_eqMagic < 0)
					{
						npc->m_eqMagic = 1;
					}
				}

				// 방어력 적용
				npc->m_eqDefense = npc->m_proto->m_defense + npc->m_proto->m_defense * nMobRate[midx][1] / 100;
				if(npc->m_eqDefense < 0)
				{
					npc->m_eqDefense = 1;
				}

				// 최대 체력 적용
				npc->m_maxHP = npc->m_proto->m_hp + npc->m_proto->m_hp * nMobRate[midx][2] / 100;
				if(npc->m_maxHP < 0)
				{
					npc->m_maxHP = 1;
				}
				npc->m_hp = npc->m_maxHP;

				// 시야 거리 적용
				if(nMobRate[midx][3] != 0 && npc->m_attackRange != 0)
				{
					npc->m_attackRange = npc->m_proto->m_attackArea + npc->m_proto->m_attackArea * nMobRate[midx][3] / 100;
				}
			}

			AddNPC(npc);
			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			CharToCell(npc, GET_YLAYER(npc), cx, cz);

			if (npc->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				GAMELOG << init("MOB REGEN")
						<< "INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NAME" << delim
						<< npc->m_name << delim
						<< "ZONE" << delim
						<< m_zone->m_index << delim
						<< "POSITION" << delim
						<< GET_X(npc) << delim
						<< GET_Z(npc) << delim
						<< GET_YLAYER(npc)
						<< end;
			}
		}
	}
}

bool CArea::RaidMobRegen(float x, float z, float r, char y)
{
	vec_raidnpcrregenlist_t::iterator it = m_raidNPCRegenList.begin();
	vec_raidnpcrregenlist_t::iterator endit = m_raidNPCRegenList.end();
	for(; it != endit; ++it)
	{
		CNPCRegenInfo* p = *(it);

		if ((p->m_lastDieTime != 0) && (gserver->m_pulse - p->m_lastDieTime >= p->m_regenSec))
		{
			// totalnum이 -1이면 무한리젠
			// totalnum이 0이면 스킵
			if (p->m_totalNum == 0)
				continue ;
			//  totalNum이 -1 아니고 numRegen이 totalNum 보다 크면 NPC가 있으면 리젠안함
			if ((p->m_totalNum != -1) && (p->m_numRegen >= p->m_totalNum))
				continue;

//#ifndef LC_MEX  브라질 설정에 맞춰야하기 때문에 주석
			int pub = GetRandom(1,100);
			if (!p->m_bRaidMoveRandom && pub <= 99)
				continue;

//#endif

			CNPC* npc;
			int cx, cz;
			npc = gserver->m_npcProtoList.Create(p->m_npcIdx, p);
			if(!npc)
			{
				p->m_totalNum = 0;
				continue;
			}

			// 속성맵상 피스존이고 피스 NPC가 아니면 리젠안함
			if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
			{
				unsigned short mapAttr = GetAttr(y, x, z);

				if(mapAttr & MATT_UNWALKABLE || mapAttr & MATT_PEACE)
				{
					delete npc;
					continue ;
				}
			}

			// 주변에 다른 레이드 몹이 있으면 그곳에는 리젠 안함
			if (this->FindProtoNPCInCell(x, z, NPC_RAID, true, CELL_EXT * 2, false) != NULL)
			{
				delete npc;
				continue ;
			}

			GET_X(npc) = x;
			GET_YLAYER(npc) = y;
			GET_Z(npc) = z;

			// 어택 가능한 NPC
			if (npc->m_proto->CheckFlag(NPC_ATTACK))
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			else
				GET_R(npc) = r;

			npc->m_regenX = GET_X(npc);
			npc->m_regenY = GET_YLAYER(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_numRegen++;

			p->m_bRaidMoveRandom = false;

			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = time(0);

			AddNPC(npc);
			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			CharToCell(npc, GET_YLAYER(npc), cx, cz);

			if (npc->m_proto->CheckFlag(NPC_RAID))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysRaidmobRegenMsg(rmsg, npc->m_proto->m_index, GET_X(npc), GET_Z(npc));
				SendToCell(rmsg, GET_YLAYER(npc), npc->m_cellX, npc->m_cellZ, RAID_NPC_NOTICE_CELL_EXT);
				GAMELOG << init("RAID MOB REGEN")
						<< "INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NAME" << delim
						<< npc->m_name << delim
						<< "ZONE" << delim
						<< m_zone->m_index << delim
						<< "POSITION" << delim
						<< GET_X(npc) << delim
						<< GET_Z(npc) << delim
						<< GET_YLAYER(npc)
						<< end;
			}

			return true;
		}
	}

	return false;
}

void MobRecover(CNPC* npc)
{
	// NPC 회복
	if (gserver->m_pulse - npc->m_recoverPulse >= NPC_RECOVER_PULSE)
	{
		if (npc->m_hp != npc->m_maxHP || npc->m_mp != npc->m_maxMP)
		{
			int hp = npc->m_hp + npc->m_recoverHP;
			int mp = npc->m_mp + npc->m_recoverMP;

			if (hp > npc->m_maxHP)
				npc->m_hp = npc->m_maxHP;
			else
				npc->m_hp = hp;

			if (mp > npc->m_maxMP)
				npc->m_mp = npc->m_maxMP;
			else
				npc->m_mp = mp;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				CharStatusMsg(rmsg, npc, 0);
				npc->m_pArea->SendToCell(rmsg, npc, false);
			}
		}

		npc->m_recoverPulse = gserver->m_pulse;
	}
}
bool MobProcSkill(CNPC* npc)
{
	bool bSkip = false;
	// 현재 스킬 사용중인가
	if(npc->m_currentSkill)
	{
		CCharacter* tskillTarget = NULL;

		switch (npc->m_currentSkill->m_state)
		{
		case SKILL_CON_NORMAL:
			break;

		case SKILL_CON_READY:
			try
			{
				// 스킬 레디->파이어 검사
				if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime <= gserver->m_pulse)
				{
					// hp, mp 검사
					if (npc->m_hp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needHP
							|| npc->m_mp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needMP)
						throw 0;

					// 타겟 검사
					if (npc->m_currentSkill->m_targetIndex != -1)
					{
						// 현재 타겟을 구해서
						tskillTarget = npc->m_pArea->FindCharInCell(npc, npc->m_currentSkill->m_targetIndex, npc->m_currentSkill->m_targetType, false);
						if (tskillTarget == NULL || tskillTarget->IsInPeaceZone(false))
							throw 0;
					}

					int	mcount = npc->m_currentSkill->m_proto->m_levelproto[npc->m_currentSkill->m_level - 1]->m_targetNum ;

					boost::scoped_array<MultiTarget> multitarget(new MultiTarget[mcount]);

					CCharacter* tcenter = (tskillTarget) ? tskillTarget : npc;
					float tr = (tskillTarget) ? atan2(GET_Z(tcenter) - GET_Z(npc), GET_X(tcenter) - GET_X(npc)) : GET_R(npc);
					float tr2;

					int sx = tcenter->m_cellX - CELL_EXT;
					int ex = tcenter->m_cellX + CELL_EXT;
					int sz = tcenter->m_cellZ - CELL_EXT;
					int ez = tcenter->m_cellZ + CELL_EXT;

					int i = 0;
					int x, z;
					bool bHelp = (npc->m_currentSkill->m_proto->m_flag & SF_HELP) ? true : false;
					bool bNotHelp = (npc->m_currentSkill->m_proto->m_flag & SF_NOTHELP) ? true : false;

					if (mcount < 1)
						mcount = 0;

					if (mcount > 0)			// 멀티 타겟 구하기 : 거리 제일 가까운 대상으로
					{
						i = 0;
						for (x = sx; x <= ex; x++)
						{
							if (x < 0 || x >= tcenter->m_pArea->m_size[0]) continue;

							for (z = sz; z <= ez; z++)
							{
								if (z < 0 || z >= tcenter->m_pArea->m_size[1]) continue;

								CCharacter *pChar;
								CCharacter *pCharNext = tcenter->m_pArea->m_cell[x][z].m_listChar;

								while ((pChar = pCharNext))
								{
									pCharNext = pCharNext->m_pCellNext;

									if ( pChar == NULL )
										throw 0;

									if (pChar->m_index <= 0)
									{
										int nIndex = 0;
										if (npc) nIndex	= npc->m_proto->m_index;

										GAMELOG << init("MOB_BUG - SORT ") << "npc " << nIndex << delim << "count " << mcount << delim
												<< "target type " << delim << pChar->m_type << delim
												<< "char index "  << delim << pChar->m_index << end;
										continue;
									}

									if(IS_NPC(pChar))
									{
										CNPC* target = TO_NPC(pChar);
#ifdef EXTREME_CUBE
										if(	   target->m_proto->m_index == CUBEMASTER_RED
											|| target->m_proto->m_index == CUBEMASTER_BLUE
											|| target->m_proto->m_index == CUBEMASTER_YELLOW
											|| target->m_proto->m_index == CUBEMASTER_PURPLE
											|| target->m_proto->m_index == CUBEMASTER_ORANGE
											|| target->m_proto->m_index == CUBE_RED
											|| target->m_proto->m_index == CUBE_BLUE
											|| target->m_proto->m_index == CUBE_YELLOW
											|| target->m_proto->m_index == CUBE_PURPLE
											|| target->m_proto->m_index == CUBE_ORANGE)
										{
											continue;
										}
#endif //EXTREME_CUBE
									}

									float tdist = GetDistance(tcenter, pChar);

									if (pChar != tcenter && !DEAD(pChar) && ((bHelp && !npc->IsEnemy(pChar)) || (bNotHelp && npc->IsEnemy(pChar))))
									{
										if (ABS(GET_YLAYER(pChar) - GET_YLAYER(tcenter)) <= 1 && tdist <= npc->m_currentSkill->m_proto->m_appRange)
										{
											// 전방120, 전방 직사각 검사
											switch (npc->m_currentSkill->m_proto->m_targetType)
											{
											case STT_TARGET_D120:
												tr2 = atan2(GET_Z(pChar) - GET_Z(npc), GET_X(pChar) - GET_X(npc));
												if (ABS(tr - tr2) > PI / 3)
													continue ;
												break;

											case STT_TARGET_RECT:
												tr2 = ABS(atan2(GET_Z(pChar) - GET_Z(npc), GET_X(pChar) - GET_X(npc)) - tr);
												if (sin(tr2) * GetDistance(GET_X(npc), GET_X(pChar), GET_Z(npc), GET_Z(pChar), 0, 0) - pChar->GetSize()> 0.6)
													continue ;
												break;
											}

											if (i == mcount -1 )
											{
												int j = 0, k =0 ;
												for (k = 1; k < mcount; k++)
												{
													if ( multitarget[j].mdist < multitarget[k].mdist )
														j = k;
												}

												if ( multitarget[j].mdist > tdist)
												{
													multitarget[j].mtarget		= pChar;
													multitarget[j].mtargettype	= (char)pChar->m_type;
													multitarget[j].mtargetindex = pChar->m_index;
													multitarget[j].mdist		= tdist;
												}
											}
											else
											{
												multitarget[i].mtarget		= pChar;
												multitarget[i].mtargettype  = (char)pChar->m_type;
												multitarget[i].mtargetindex = pChar->m_index;
												multitarget[i].mdist		= tdist;
												i++;
											}
										}
									}
								}
							}
						}
					}

					mcount = i;

					int m = 0;
					for (m = 0; m < mcount; m++)
					{
						if (multitarget[m].mtarget == NULL && multitarget[m].mtargettype != MSG_CHAR_UNKNOWN)
						{
							int nIndex = 0;
							if (npc) nIndex	= npc->m_proto->m_index;

							GAMELOG << init("MOB_BUG - targer is NULL ") << "npc " << nIndex << delim << "i " << m << delim << "count " << mcount << delim
									<< "target type " << delim << multitarget[m].mtargettype << delim
									<< "char index "  << delim << multitarget[m].mtargetindex << end;
							continue;
						}

						if (multitarget[m].mtarget == NULL)
							continue;

						if (multitarget[m].mtarget->m_index <= 0)
						{
							int nIndex = 0;
							if (npc) nIndex	= npc->m_proto->m_index;

							GAMELOG << init("MOB_BUG - wrong index ") << "npc " << nIndex << delim << "i " << m << delim << "count " << mcount << delim
									<< "target type " << delim << multitarget[m].mtargettype << delim
									<< "char index "  << delim << multitarget[m].mtargetindex << end;
							continue;
						}
					}

					npc->m_currentSkill->Fire();
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillFireMsg(rmsg, npc, npc->m_currentSkill, tskillTarget, mcount, multitarget.get(), 0, 0.0f, 0.0f, 0.0f, 0.0f, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					npc->m_hp -= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needHP;
					npc->m_mp -= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needMP;
					{
						CNetMsg::SP rmsg(new CNetMsg);
						CharStatusMsg(rmsg, npc, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					// 스킬 적용
					// 타게팅에 따라
					bool bApply;
					switch (npc->m_currentSkill->m_proto->m_targetType)
					{
					case STT_SELF_ONE:
						ApplySkill(npc, npc, npc->m_currentSkill, -1, bApply);
						break;

					case STT_SELF_RANGE:
						{
							if (ApplySkill(npc, npc, npc->m_currentSkill, -1, bApply) != -1)
							{
								for (i = 0; i < mcount; i++)
								{
									if (multitarget[i].mtarget->m_index <= 0)
									{
										int nIndex = 0;
										if (npc) nIndex	= npc->m_proto->m_index;

										GAMELOG << init("MOB_BUG - STT_SELF_RANGE ") << "npc " << nIndex << delim << "i " << i << delim << "count " << mcount << delim
												<< "target type " << delim << multitarget[i].mtargettype << delim
												<< "char index "  << delim << multitarget[i].mtargetindex << end;
										break;
									}
									// npc가 테이밍된 상태이면
									if ( npc->Check_MobFlag( STATE_MONSTER_TAMING )
											|| npc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
									{
										const CSkillProto* proto = npc->m_currentSkill->m_proto;

										// 스킬이 해꼬지 타입이면
										if ( proto && proto->m_flag & SF_NOTHELP && multitarget[i].mtargettype != MSG_CHAR_NPC )
										{
											// NPC만 데미지를 입는다.
											continue;
										}
									}
									if (ApplySkill(npc, multitarget[i].mtarget, npc->m_currentSkill, -1, bApply) == -1)
										break;
								}
							}
						}
						break;

					case STT_TARGET_ONE:
						if (tskillTarget)
							ApplySkill(npc, tskillTarget, npc->m_currentSkill, -1, bApply);
						break;

					case STT_TARGET_RANGE:
					case STT_TARGET_D120:
					case STT_TARGET_RECT:
						{
							if (tskillTarget)
							{
								if (ApplySkill(npc, tskillTarget, npc->m_currentSkill, -1, bApply) != -1)
								{
									for (i = 0; i < mcount; i++)
									{
										if (multitarget[i].mtarget->m_index <= 0)
										{
											int nIndex = 0;
											if (npc) nIndex	= npc->m_proto->m_index;

											GAMELOG << init("MOB_BUG - STT_TARGET_RANGE ") << "npc " << nIndex << delim << "i " << i << delim << "count " << mcount << delim
													<< "target type " << delim << multitarget[i].mtargettype << delim
													<< "char index "  << delim << multitarget[i].mtargetindex << end;
											break;
										}
										// npc가 테이밍된 상태이면
										if ( npc->Check_MobFlag( STATE_MONSTER_TAMING )
												|| npc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
										{
											const CSkillProto* proto = npc->m_currentSkill->m_proto;

											// 스킬이 해꼬지 타입이면
											if ( proto && proto->m_flag & SF_NOTHELP && multitarget[i].mtargettype != MSG_CHAR_NPC )
											{
												// NPC만 데미지를 입는다.
												continue;
											}
										}
										if (ApplySkill(npc, multitarget[i].mtarget, npc->m_currentSkill, -1, bApply) == -1)
											break;
									}
								}
							}
						}
						break;

					case STT_PARTY_ONE:
						if (tskillTarget)
						{
							if (npc->m_proto->m_family != -1 && IS_NPC(tskillTarget) && npc->m_proto->m_family == TO_NPC(tskillTarget)->m_proto->m_family)
								ApplySkill(npc, tskillTarget, npc->m_currentSkill, -1, bApply);
						}
						break;

					case STT_PARTY_ALL:
						if (npc->m_proto->m_family != -1)
						{
							int sx = npc->m_cellX - CELL_EXT;
							int ex = npc->m_cellX + CELL_EXT;
							int sz = npc->m_cellZ - CELL_EXT;
							int ez = npc->m_cellZ + CELL_EXT;

							int x, z;

							for (x = sx; x <= ex; x++)
							{
								if (x < 0 || x >= npc->m_pArea->m_size[0])
									continue;

								for (z = sz; z <= ez; z++)
								{
									if (z < 0 || z >= npc->m_pArea->m_size[1])
										continue;

									CCharacter *pChar;
									CCharacter *pCharNext = npc->m_pArea->m_cell[x][z].m_listChar;

									while ((pChar = pCharNext))
									{
										pCharNext = pCharNext->m_pCellNext;

										if (IS_NPC(pChar) && TO_NPC(pChar)->m_proto->m_family == npc->m_proto->m_family)
										{
											if (ApplySkill(npc, pChar, npc->m_currentSkill, -1, bApply) == -1)
												break;
										}
									}
								}
							}
						}
						break;
					}
					if( npc->Check_MobFlag(STATE_MONSTER_SUICIDE) || npc->Check_MobFlag(STATE_MONSTER_TRAP) )
					{
						if( mcount > 0 || tskillTarget )  // 한번이라도 스킬을 사용했다면
						{
							npc->GetOwner()->SummonNpcRemove(npc);
							bSkip = true;
						}
					}
				} // end if

				npc->m_aipulse = gserver->m_pulse;
				npc->m_delay = NPC_ACTIVITY_DELAY;
			}
			catch (...)
			{
				/* speed_ksw after */
				CNetMsg::SP rmsg(new CNetMsg);
				/* speed_ksw after */
				npc->m_currentSkill->Cancel(npc);
				npc->m_currentSkill = NULL;
				ResponseClient::makeSkillCancelMsg(rmsg, npc);
				npc->m_pArea->SendToCell(rmsg, npc, false);
			}
			bSkip = true;
			break;

		case SKILL_CON_FIRE:
			if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime + npc->m_currentSkill->m_proto->m_fireTime <= gserver->m_pulse)
			{
				npc->m_currentSkill->Cancel(npc);
				npc->m_currentSkill = NULL;
				npc->m_aipulse = gserver->m_pulse;
				npc->m_delay = NPC_ACTIVITY_DELAY;
			}
			else
				bSkip = true;
			break;
		}
	}

	return bSkip;
}

CCharacter* MobSelectSkill(CNPC* npc, CCharacter* target)
{
	// 현재 스킬 사용중이면 스킵
	if (npc->m_currentSkill)
		return target;

	bool bHealerFlag2_Mercenary = ( npc->m_proto->m_aitype == NPC_AI_HEALER && npc->m_proto->CheckAIBit(2) );
	if ( bHealerFlag2_Mercenary && ( npc->Check_MobFlag( STATE_MONSTER_MERCENARY) ) )
	{
		if( npc->GetOwner() && npc->GetOwner()->m_hp < npc->GetOwner()->m_maxHP )
		{
			target = (CCharacter *)npc->GetOwner();
			return target;
		}
		else
			target = npc->GetOwner()->GetSummonOwners_target();
	}
	else if( npc->Check_MobFlag( STATE_MONSTER_MERCENARY) && npc->GetOwner() )
	{
		target = npc->GetOwner()->GetSummonOwners_target();
	}

	if( npc->Check_MobFlag( STATE_MONSTER_MERCENARY) )
	{
		if( !npc->GetOwner() )
			return NULL;

		if( (target && ( target->m_pZone->m_index != npc->m_pZone->m_index || GetDistance(target, (CCharacter*)npc) >= 40.0f ))
				|| GetDistance( (CCharacter*)npc->GetOwner(), (CCharacter*)npc) > 40.0f ) // 같은 존에 있지 않거나 거리가 너무 멀면 오너와 너무 멀어지면 공격을 취소하자.
		{
			npc->GetOwner()->SetSummonOwners_target(NULL);
			return NULL;
		}
	}

	// 회복 스킬을 가진 npc가 테이밍 되었을 경우
	// 테이밍 몬스터 이면서 힐러는 플래그 2일때 HP 80% 미만의 자신과 PC에게 즉효성 스킬 사용
	//bool bHealerFlag2_Taming = ( npc->m_proto->m_aitype == NPC_AI_HEALER && npc->m_proto->CheckAIBit(2) );
	if ( /*bHealerFlag2_Taming &&*/ ( npc->Check_MobFlag( STATE_MONSTER_TAMING ) ) )
	{
		int sx1 = npc->m_cellX - CELL_EXT;
		int ex1 = npc->m_cellX + CELL_EXT;
		int sz1 = npc->m_cellZ - CELL_EXT;
		int ez1 = npc->m_cellZ + CELL_EXT;

		int a, b;

		// 일단 다른 층은 검사 안한다
		for (a = sx1; a <= ex1; a++)
		{
			if (a < 0 || a >= npc->m_pArea->m_size[0]) continue;
			for (b = sz1; b <= ez1; b++)
			{
				if (b < 0 || b >= npc->m_pArea->m_size[1]) continue;
				CCharacter *pChar;
				for (pChar = npc->m_pArea->m_cell[a][b].m_listChar; pChar; pChar = pChar->m_pCellNext)
				{
					// 죽은넘 제외, 다른층 제외
					if ( DEAD(pChar) || ABS( GET_YLAYER(npc) - GET_YLAYER(pChar) ) >= 2)
						continue;

					// npc이면 자기 자신 말고는 제외
					if ( IS_NPC(pChar) )
					{
						CNPC* check_npc = TO_NPC(pChar);
						if ( check_npc != npc )
						{
							continue;
						}
					}

					// 스킬 사용 가능 검사
					int skillindex = -1;
					int i;
					for (i = 0; i < MAX_NPC_SKILL; i++)
					{
						if (npc->m_skills[i]
								&& (npc->m_skills[i]->m_proto->m_flag & SF_HELP)
								&& npc->m_skills[i]->IsReady(npc)
								&& npc->CanSpell()
								&& npc->m_hp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needHP
								&& npc->m_mp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needMP
								&& pChar->CanApplySkill(npc->m_skills[i]->m_proto, npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level))
								&& (npc->CanMove() || GetDistance(npc, pChar) - npc->m_skills[i]->m_proto->GetFireRange(npc) <= 0.3))
						{
							// AI Type에 따라 사용할 스킬 종류 결정(지속, 즉효)
							if (    ( //bHealerFlag2_Taming &&
										npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_durtime < 1
										&& pChar->m_hp < pChar->m_maxHP * 80 / 100)										// 힐러 플래그2는 즉효성만 체력부족 대상에게 걸어줌
							   )
							{
								skillindex = i;
								break;
							}
						}
					}

					// 스킬 대상 선정 마지막
					if ( skillindex >= 0 )
					{
						target = pChar;
						npc->m_currentSkill = npc->m_skills[skillindex];
						npc->m_currentSkill->m_targetType = target->m_type;
						npc->m_currentSkill->m_targetIndex = target->m_index;
						return target;
					}
				}
			}  // for
		}  // for
	}  // if

	// 데미지딜러는 플래그 2일때 주변 동족에게 버프를 걸어준다
	// 힐러는 플래그 0일때 주변 동족에게 버프를 걸어준다
	// 힐러는 플래그 2일때 주변 동족(자신포함) HP 80% 미만에게 즉효성 스킬 사용
	bool bDamageDealerFlag2 = (target && npc->m_proto->m_aitype == NPC_AI_DAMAGEDEALER && npc->m_proto->CheckAIBit(2) && npc->m_hp >= npc->m_maxHP * 80 / 100 && npc->m_proto->m_family != -1);
	bool bHealerFlag0 = (target && npc->m_proto->m_aitype == NPC_AI_HEALER && npc->m_proto->CheckAIBit(0) && npc->m_hp >= npc->m_maxHP * 80 / 100 && npc->m_proto->m_family != -1);
	bool bHealerFlag2 = (npc->m_proto->m_aitype == NPC_AI_HEALER && npc->m_proto->CheckAIBit(2) && npc->m_proto->m_family != -1);
	if (bDamageDealerFlag2 || bHealerFlag0 || bHealerFlag2)
	{
		int sx = npc->m_cellX - CELL_EXT;
		int ex = npc->m_cellX + CELL_EXT;
		int sz = npc->m_cellZ - CELL_EXT;
		int ez = npc->m_cellZ + CELL_EXT;

		int x, z;
		float dist = 999.9f;
		CNPC* ret = NULL;
		CSkill* retSkill = NULL;

		// 일단 다른 층은 검사 안한다
		for (x = sx; x <= ex; x++)
		{
			if (x < 0 || x >= npc->m_pArea->m_size[0]) continue;
			for (z = sz; z <= ez; z++)
			{
				if (z < 0 || z >= npc->m_pArea->m_size[1]) continue;
				CCharacter *pChar;
				for (pChar = npc->m_pArea->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
				{
					// NPC만, 죽은넘 제외, 다른층 제외
					if (!IS_NPC(pChar) || DEAD(pChar) || ABS(GET_YLAYER(npc) - GET_YLAYER(pChar)) >= 2)
						continue ;

					CNPC* tnpc = TO_NPC(pChar);

					// 동족이고
					if (tnpc->m_proto->m_family != npc->m_proto->m_family)
						continue ;

					// 스킬 사용 가능 검사
					int skillindex = -1;
					int i;
					for (i = 0; i < MAX_NPC_SKILL; i++)
					{
						if (npc->m_skills[i]
								&& (npc->m_skills[i]->m_proto->m_flag & SF_HELP)
								&& npc->m_skills[i]->IsReady(npc)
								&& npc->CanSpell()
								&& npc->m_hp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needHP
								&& npc->m_mp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needMP
								&& tnpc->CanApplySkill(npc->m_skills[i]->m_proto, npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level))
								&& (npc->CanMove() || GetDistance(npc, tnpc) - npc->m_skills[i]->m_proto->GetFireRange(npc) <= 0.3))
						{
							// AI Type에 따라 사용할 스킬 종류 결정(지속, 즉효)
							if (    (bDamageDealerFlag2
									 && npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_durtime > 0
									 && !tnpc->m_assist.FindBySkillIndex(npc->m_skills[i]->m_proto->m_index))		// 대미지 딜러는 버프만 안걸린 대상에게 걸어줌

									|| (bHealerFlag0
										&& npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_durtime > 0
										&& !tnpc->m_assist.FindBySkillIndex(npc->m_skills[i]->m_proto->m_index))		// 힐러 플래그0는 버프만 안걸린 대상에게 걸어줌

									|| (bHealerFlag2
										&& npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_durtime < 1
										&& tnpc->m_hp < tnpc->m_maxHP * 80 / 100)										// 힐러 플래그2는 즉효성만 체력부족 대상에게 걸어줌
							   )
							{
								skillindex = i;
								break;
							}
						}
					}

					// 스킬 대상 선정 마지막
					if (skillindex >= 0)
					{
						// 기존과 타입검사 : 탱커, 자신의타입, 그외, 없음
						// 타입이 같으면 거리로 비교
						int resComp = AIComp(npc, tnpc, ret);
						if (resComp < 0 || (resComp == 0 && dist > GetDistance(npc, tnpc)))
						{
							dist = GetDistance(npc, tnpc);
							ret = tnpc;
							retSkill = npc->m_skills[skillindex];
						}
					}
				}
			}
		}

		if (ret)
		{
			target = ret;
			npc->m_currentSkill = retSkill;
			npc->m_currentSkill->m_targetType = target->m_type;
			npc->m_currentSkill->m_targetIndex = target->m_index;
			return target;
		}
	}

	// 레이드 몹 분노 처리 : 체력이 50% 이하로 떨어질때
	if (npc->m_proto->CheckFlag(NPC_RAID) && npc->m_hp <= npc->m_maxHP / 2)
	{
		// 아직 분노 스킬을 사용하지 않았을 때
		if (npc->m_assist.FindBySkillIndex(RAID_NPC_RAGE_SKILL) == 0)
		{
			npc->m_currentSkill = gserver->m_skillProtoList.Create(RAID_NPC_RAGE_SKILL, RAID_NPC_RAGE_SKILL_LEVEL);
			if (npc->m_currentSkill)
			{
				npc->m_currentSkill->m_targetType = npc->m_type;
				npc->m_currentSkill->m_targetIndex = npc->m_index;
				return npc;
			}
		}
	}

	// 타겟이 있을 때 : 이경우는 적에게만 적용
	if (target)
	{
		// 혼란이나 테이밍된 npc는 unhelp타입 스킬 타겟을 pc로 잡을 수 없다.
		if( IS_PC(target)
				&& ( npc->Check_MobFlag( STATE_MONSTER_TAMING ) || npc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
		  )
			return target;

		if(DEAD(target))
			return NULL;

		float dist = GetDistance((CCharacter*)npc, target);
		if (!npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
		{
			int skillindex = -1;
			int i;
			for (i = 0; i < MAX_NPC_SKILL; i++)
			{
				int skillprob = GetRandom(1, 10000);
				// 스킬 사용 가능 검사
				if (npc->m_skills[i]
						&& npc->m_proto->m_skillProb[i] >= skillprob
						&& npc->m_skills[i]->IsReady(npc)
						&& npc->CanSpell()
						&& npc->m_hp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needHP
						&& npc->m_mp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needMP
						&& target->CanApplySkill(npc->m_skills[i]->m_proto, npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level))
						&& (npc->CanMove() || dist - npc->m_skills[i]->m_proto->GetFireRange(npc) <= 0.3))
				{
					skillindex = i;
					break;
				}
			}
			if (skillindex >= 0)
			{
				npc->m_currentSkill = npc->m_skills[skillindex];
				npc->m_currentSkill->m_targetType = target->m_type;
				npc->m_currentSkill->m_targetIndex = target->m_index;
			}
		}
	}

	return target;
}

bool MobReadySkill(CNPC* npc, CCharacter* target)
{
	if (npc->m_currentSkill && npc->m_currentSkill->m_state == SKILL_CON_NORMAL && GetDistance(npc, target) - npc->m_currentSkill->m_proto->GetFireRange(npc) <= 0.3)
	{
		if( !npc->Check_MobFlag( STATE_MONSTER_TRAP ))
			npc->CancelInvisible();

		switch (npc->m_currentSkill->m_proto->m_targetType)
		{
		case STT_SELF_ONE:
		case STT_SELF_RANGE:
		case STT_PARTY_ONE:
		case STT_PARTY_ALL:
			npc->m_currentSkill->Ready(NULL);
			break;

		default:
			npc->m_currentSkill->Ready(target);
			break;
		}

#if defined (LC_GAMIGO)  || defined(LC_BILA)
		// imso- NPC_RAID BUG
		if (!npc->m_proto->CheckFlag(NPC_RAID))
#endif
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillReadyMsg(rmsg, npc, npc->m_currentSkill, target);
			npc->m_pArea->SendToCell(rmsg, npc, false);
		}
		npc->m_aipulse = gserver->m_pulse;
		npc->m_delay = 0;

		return true;
	}
	return false;
}

bool MobProcFear(CNPC* npc, CCharacter* target)
{
	// 피어 걸린 NPC인가? : 피스NPC, 이동불가 NPC는 피어 무시
	if (npc->m_proto->CheckFlag(NPC_PEACEFUL) ||
			!npc->m_proto->CheckFlag(NPC_MOVING) ||
			(target == NULL && npc->m_assist.m_avAddition.hcFearIndex < 1))
		return false;

	// 피어 대상 찾기
	if (target == NULL && npc->m_assist.m_avAddition.hcFearIndex > 0)
	{
		target = npc->m_pArea->FindCharInCell(npc, npc->m_assist.m_avAddition.hcFearIndex, npc->m_assist.m_avAddition.hcFearType, false);

		// 피어 대상이 시야내에 없으면 풀기
		if (!target)
		{
			npc->m_assist.CureAssist(MST_ASSIST_FEAR, 99);
			return false;
		}
	}

	// 피어의 대상으로부터 도망
	float tr = (float)atan2(GET_Z(npc) - GET_Z(target), GET_X(npc) - GET_X(target));
	float dist = npc->GetRunSpeed();
	float x = GET_X(npc) + dist * (float)cos(tr);
	float z = GET_Z(npc) + dist * (float)sin(tr);
	MobMoveTo(npc, x, z, true, false, target);

	return true;
}

float MobMoveTo(CNPC* npc, float x, float z, bool bRun, bool bAttack, CCharacter* target)
{
	float ox = x;
	float oz = z;
	float dist = 0;
	float dist_for_attack = 0.0f;

	if (!npc->CanMove())
	{
		dist = GetDistance(ox, oz, npc->m_pArea->GetHeight(GET_YLAYER(npc), ox, oz), npc);
		if (target)
			dist -= target->GetSize();
		if (npc->m_currentSkill)
			dist -= npc->m_currentSkill->m_proto->GetFireRange(npc);
		else if (target && bAttack && npc->CanAttack())
			dist -= npc->m_attackRange;

		return dist;
	}

	// 뛰기모드 설정
	npc->m_bMoveRun = bRun;
	// 최대 이동 거리를 이속에서 가져옴
	float moveSpeed = (bRun) ? npc->GetRunSpeed() : npc->m_walkSpeed;

	// 방향
	float tr = (float)atan2(z - GET_Z(npc), x - GET_X(npc));

	// 대상 지점의 높이
	float h = npc->m_pArea->GetHeight(GET_YLAYER(npc), x, z);

	float trh = 0.0f;
	bool bRunAI = false;

	char outY;

	// 이동시 블럭물이 있는지 검사
	if (npc->m_pArea->IsNotBlocked(npc, GET_YLAYER(npc), x, z, bAttack, outY))
	{
		// 타겟과의 거리 검사 H 포함
		dist = GetDistance(x, z, h, npc);
		if (target)
			dist -= target->GetSize();
		if (npc->m_currentSkill)
			dist -= npc->m_currentSkill->m_proto->GetFireRange(npc);
		else if (target && bAttack && npc->CanAttack())
			dist -= npc->m_attackRange;

		dist_for_attack = dist;

		// 이동 거리가 0.3 미만이면 무시
		if (dist <= 0.3f)
			return 0.0f;

		// 이동거리보다 작으면 딜레이를 반으루 줄여서 이동
		if (dist < moveSpeed)
		{
			if (dist < (moveSpeed / 2))
				npc->m_delay  = 0;
		}

		float fValidCheck = (h - GET_H(npc)) / (GetDistance(x, GET_X(npc), z, GET_Z(npc), h, GET_H(npc)) + 0.01f);
		if (fValidCheck > 1.0f)
			fValidCheck = 1.0f;
		trh = (float)asin(fValidCheck);
	}
	else
	{
		bRunAI = true;

		dist_for_attack = 9999.0f;

		// AI 이동경로에서 블럭당한적이 있고 방향 바꿔야 하면 방향 전환
		if (npc->m_nBlockAI != 0 && npc->m_bChangeDir)
			npc->m_moveDir *= -1;

		// 양쪽방향으로 바꾸면서 시도 횟수 3번까지 초과시 랜덤이동(타겟이 없는것처럼)
		if (npc->m_nBlockAI > 3)
		{
			tr = GetRandom((int)(-PI* 1000), (int)(PI * 1000)) / 1000.0f;
			dist = npc->m_walkSpeed;
			npc->m_bMoveRun= false;

			// AI관련변수 초기화
			npc->InitAIVar();
		}
		// 90도꺽기
		else
		{
			if (npc->m_bFail45)
				// Create 시 정해진 방향으로 90도 회전 45도 회전이 실패일경우 시도
				tr += npc->m_moveDir * PI_HALF;
			else
				// 정해진 방향으로 45도 회전
				tr += npc->m_moveDir * PI_HALF / 2;

			// 방향 안바꿔도 된다
			npc->m_bChangeDir = false;

			// runSpeed 만큼 이동
			//dist = npc->m_runSpeed;
			npc->m_bMoveRun =true;
			dist = 1.0f;
			npc->m_delay = 0;
		}

		// 높이적용
		x = GET_X(npc) + dist * (float)cos(tr);
		z = GET_Z(npc) + dist * (float)sin(tr);
		h = npc->m_pArea->GetHeight(GET_YLAYER(npc), x, z);
		float fValidCheck = (h - GET_H(npc)) / (GetDistance(x, GET_X(npc), z, GET_Z(npc), h, GET_H(npc)) + 0.01f);
		if (fValidCheck > 1.0f)
			fValidCheck = 1.0f;
		trh = (float)asin(fValidCheck);
	}

	npc->m_bStop = false;

	if (npc->m_bMoveRun)
	{
		if (dist > npc->GetRunSpeed())
			dist = npc->GetRunSpeed();
	}
	else
	{
		if (dist > npc->m_walkSpeed)
			dist = npc->m_walkSpeed;
	}
	float dist2 = dist * (float)cos(trh);

	x = GET_X(npc) + dist2 * (float)cos(tr);
	z = GET_Z(npc) + dist2 * (float)sin(tr);

	bool bTeleportToRegen = false;

	// 이동 영역 안인가 보고
	if (!target)
	{
		if (ABS(npc->m_regenX - x) > npc->m_moveArea || ABS(npc->m_regenZ - z) > npc->m_moveArea)
			bTeleportToRegen = true;
		// 타겟이 없고 이동영역 안에 있는 경우
		else
			npc->m_nBlockRegen = 0;
	}

	// 평화존에 있을 경우 무조건 리젠 위치를 향해 돌아간다
	if (npc->IsInPeaceZone(true))
		bTeleportToRegen = true;

	if (bTeleportToRegen)
	{
		if (npc->m_nBlockRegen > 3)
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DisappearMsg(rmsg, npc);
				npc->m_pArea->SendToCell(rmsg, npc, false);
			}

			npc->m_pArea->CharFromCell(npc, true);

			GET_X(npc) = npc->m_regenX;
			GET_YLAYER(npc) = npc->m_regenY;
			GET_Z(npc) = npc->m_regenZ;

			int cx, cz;
			npc->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			npc->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, false);
				npc->m_pArea->SendToCell(rmsg, npc, false);
			}

			// AI 관련 변수 초기화
			npc->InitAIVar();

			return 999.9f;
		}
		else
		{
			// 영역 밖이면 리젠 위치를 향해 이동하고
			tr = (float)atan2(npc->m_regenZ - GET_Z(npc), npc->m_regenX - GET_X(npc));

			x = GET_X(npc) + (float)cos(tr) * npc->GetRunSpeed();
			z = GET_Z(npc) + (float)sin(tr) * npc->GetRunSpeed();
			float h = npc->m_pArea->GetHeight(GET_YLAYER(npc), x, z);

			// 높이값 적용
			float fValidCheck = (h - GET_H(npc)) / (GetDistance(x, GET_X(npc), z, GET_Z(npc), h, GET_H(npc)) + 0.01f);
			if (fValidCheck > 1.0f)
				fValidCheck = 1.0f;
			trh = (float)asin(fValidCheck);

			dist2 = dist * (float)cos(trh);
			x = GET_X(npc) + dist2 * (float)cos(tr);
			z = GET_Z(npc) + dist2 * (float)sin(tr);

			npc->m_bMoveRun = true;
		}
	}

	if (npc->m_bMoveRun)
	{
		if (!npc->m_pArea->MoveChar(npc, outY, x, z, npc->m_pArea->GetHeight(outY, x, z), tr, MSG_MOVE_RUN, target))
		{
			// AI적용 시 Move를 실패한경우 카운트 증가 방향전환 셋팅
			if (bRunAI)
			{
				npc->m_nBlockAI++;
				npc->m_bChangeDir = true;
			}

			// 45도 성공이었다면 실패로
			if (!npc->m_bFail45)
				npc->m_bFail45 = true;

			// 리젠 시 실패 된 경우
			if (bTeleportToRegen)
				npc->m_nBlockRegen++;

			npc->m_bStop = true;
		}
	}
	else
	{
		if (!npc->m_pArea->MoveChar(npc, outY, x, z, npc->m_pArea->GetHeight(outY, x, z), tr, MSG_MOVE_WALK, target))
		{
			npc->m_bStop = true;
		}
	}	// if (npc->m_bMoveRun)

	dist = GetDistance(ox, oz, npc->m_pArea->GetHeight(GET_YLAYER(npc), ox, oz), npc);
	if (target)
		dist -= target->GetSize();
	if (npc->m_currentSkill)
		dist -= npc->m_currentSkill->m_proto->GetFireRange(npc);
	else if (target && bAttack && npc->CanAttack())
		dist -= npc->m_attackRange;

	return dist;
}

CCharacter* MobSelectTarget(CNPC* npc)
{
	CCharacter* target = NULL;

	if (npc->m_attackList)
		target = CheckAttackPulse(npc);

	// 평화 NPC는 타겟 선전 안함
	if (npc->m_proto->CheckFlag(NPC_PEACEFUL))
		return NULL;

	// 데미지 딜러의 HP가 80% 이상일때
	bool bDamageDealerFlag1 = (npc->m_proto->m_aitype == NPC_AI_DAMAGEDEALER && (target == NULL || npc->m_hp >= npc->m_maxHP * 80 / 100) && npc->m_proto->CheckAIBit(1));
	bool bDamageDealerFlag0 = (npc->m_proto->m_aitype == NPC_AI_DAMAGEDEALER && (target == NULL || npc->m_hp >= npc->m_maxHP * 80 / 100) && npc->m_proto->CheckAIBit(0));
	// 힐러 flag 1
	bool bHealerFlag0 = (npc->m_proto->m_aitype == NPC_AI_HEALER && (target == NULL || npc->m_hp >= npc->m_maxHP * 80 / 100) && npc->m_proto->CheckAIBit(1));
	if (bDamageDealerFlag1 || bDamageDealerFlag0 || bHealerFlag0)
	{
		CNPC* familyForHelp = NULL;

		// 대미지 딜러 flag 1 에서 주변에 힐러의 체력이 60%미만이면 힐러 지원
		if (familyForHelp == NULL && bDamageDealerFlag1)
			familyForHelp = npc->m_pArea->FindNearestFamily(npc, NPC_AI_HEALER, 60);

		// 대미지 딜러 flag 0/힐러 flag 1 에서 주변에 탱커 타겟을 지원
		if (familyForHelp == NULL && (bDamageDealerFlag0 || bHealerFlag0))
			familyForHelp = npc->m_pArea->FindNearestFamily(npc, NPC_AI_TANKER);

		if (familyForHelp)
		{
			CCharacter* targetOfFamily = MobSelectTarget(familyForHelp);
			if (targetOfFamily)
			{
				if (target == NULL)
				{
					AddAttackList(npc, targetOfFamily, HATE_FIRST_ATTACK);
				}
				target = targetOfFamily;
			}
		}
	}

	// 수호탑은 언제나 새로 선정
	if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
		target = NULL;

	// 타겟 없고
	// 선공이면 주변 캐릭터 찾기
	float dist = 999.9f;
	if (!npc->m_pArea->IsFreeze(gserver->m_pulse))
	{
		if (!target && npc->m_proto->CheckFlag(NPC_FIRSTATTACK))
		{
			if (npc->m_disableTime == 0 && !npc->IsBlind())
			{
				target = npc->m_pArea->FindNearestTarget(npc, &dist);

				// 피스존 안에서는 타겟이 될 수 없다
				if (target && target->IsInPeaceZone(false))
					target = NULL;
								
				if (target && 
					target->m_pZone != NULL &&
					target->m_pZone->isRVRZone() == true &&
					target->getSyndicateType() == npc->getSyndicateType())
				{
					target = NULL;
				}

				if (target)
					AddAttackList(npc, target, HATE_FIRST_ATTACK);
			}
		}

		if (!target && npc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
		{
			if (npc->m_disableTime == 0 && !npc->IsBlind())
			{
				target = npc->m_pArea->FindNearestTarget(npc, &dist);
				if (target)
					AddAttackList(npc, target, HATE_FIRST_ATTACK);
			}
		}
	}

	if( !target && ( TO_NPC(npc)->Check_MobFlag(STATE_MONSTER_SUICIDE) || TO_NPC(npc)->Check_MobFlag(STATE_MONSTER_TRAP) ) )
	{
		target = npc->m_pArea->FindNearestTarget(npc, &dist);
	}

	if( target && IS_NPC(target) && TO_NPC(target)->GetOwner()  )
	{
		if( TO_NPC(target)->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK) || TO_NPC(target)->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF) )
		{
			target = (CCharacter*)TO_NPC(target)->GetOwner();
		}
	}

	return target;
}

CCharacter* MobSelectTarget_Taming_or_Confused(CNPC* npc)
{
	CCharacter* target = NULL;

	// 만약 계속 목표가 바뀌면 이것 열어야 함
	//if (npc->m_attackList)
	//	target = CheckAttackPulse(npc);

	// 몬스터가 몬스터를 때림
	float dist = 999.9f;
	if (!target)
	{
		if (npc->m_disableTime == 0 && !npc->IsBlind())
		{
			target = npc->m_pArea->FindNearestTarget_Taming_or_Confused(npc, &dist);

			// 피스존 안에서는 타겟이 될 수 없다
			if (target && target->IsInPeaceZone(false))
				target = NULL;

			if (target)
				AddAttackList(npc, target, HATE_FIRST_ATTACK);
		}
	}

	return target;
}

void MobMoveRandom(CNPC* npc)
{
	// 이동불가능
	if (!npc->CanMove())
		return ;

	// 20%의 확률로 랜덤 움직임
	// 리젠 직후 상태이면 100% 랜덤 움직임(안보임)
	// 10%의 확률로 랜덤이동
	/*speed_ksw before*/
	if ((npc->m_disableTime !=0) || (GetRandom(1, 100) <= 20))
		/*speed_ksw before*/
		/*
		speed_ksw after
		if ((npc->m_disableTime !=0) || (GetRandom(1, 10000) <= 500))
		speed_ksw after
		*/
	{
		float tr = GetRandom((int)(-PI* 1000), (int)(PI * 1000)) / 1000.0f;
		npc->m_bMoveRun = false;
		// 높이적용
		float x = GET_X(npc) + (npc->m_walkSpeed + npc->m_proto->m_size) * (float)cos(tr);
		float z = GET_Z(npc) + (npc->m_walkSpeed + npc->m_proto->m_size) * (float)sin(tr);
		MobMoveTo(npc, x, z, false, false, NULL);
	}
}

void CArea::MobActivity_Taming(CNPC* npc)
{
	CPC*	owner = NULL;				// 몬스터를 테이밍한 캐릭터
	CNPC*	target_tamingNpc = NULL;	// 테이밍된 몬스터가 노리는 몬스터
	//CPC*	target_for_heal = NULL;		// npc가 힐을 해주는 PC
	CCharacter* temp = NULL;

	// 주인을 찾는다.
	owner = npc->GetOwner();
	if (owner == NULL)
	{
		npc->reSet_MobFlag( STATE_MONSTER_TAMING );
		npc->m_assist.CureAssist(MST_ASSIST_TAMING, 99);
		DelAttackList(npc);
		npc->SetOwner(NULL);
		MobMoveRandom(npc);
		return ;
	}

	// 주인이 죽거나 멀리 떨어지면(100미터) 테이밍이 풀리고 랜덤 이동
	if ( owner->m_hp <= 0 ) // || GetDistance(owner, npc) >= 100.0f )
	{
		npc->reSet_MobFlag( STATE_MONSTER_TAMING );
		npc->m_assist.CureAssist(MST_ASSIST_TAMING, 99);
		DelAttackList(npc);
		MobMoveRandom(npc);

		// 주인이 있는지 확인, 있으면 테이밍 중을 해제
		owner->DeleteSlave(npc);

		// npc의 주인도 지워 줌
		npc->SetOwner(NULL);
		return;
	}
	else
	{
		// 주인이 공격한 몬스터를 공격한다.
		temp = owner->GetOwners_target();
	}

	// 초기에는 주변의 일반 몬스터를 찾아서 선공이었다... 지금은 주인이 공격한 몬스터를 공격한다.
	// 주변 몬스터 선공으로 바꾸자 하면 이것의 주석을 해제하면 된다.    yhj
	//temp = MobSelectTarget_Taming_or_Confused(npc);

	/* //  임시로 테이밍된 npc는 스킬을 사용 못하게 막는다.
	// 몹 스킬 선택
	temp = MobSelectSkill(npc, temp); */

	if ( temp && IS_NPC(temp) )
	{
		target_tamingNpc = TO_NPC(temp);
	}

	/*
	else if( temp && IS_PC(temp) )
	{
		// 힐이면 타겟을 PC로 잡는다.
		target_for_heal = TO_PC(temp);
	}

	// 힐이면 타겟에게 스킬을 사용한다.
	if ( target_for_heal )
	{
		// 스킬은 바로 사용한다.
		MobReadySkill(npc, target_for_heal);
		return;
	}

	else *///  임시로 테이밍된 npc는 스킬을 사용 못하게 막는다.

	if( target_tamingNpc && target_tamingNpc != npc)
	{
		// 타겟이 있으면 타겟을 향해 이동
		float dist = MobMoveTo(npc, GET_X(target_tamingNpc), GET_Z(target_tamingNpc), true, true, (CCharacter*)target_tamingNpc);

		if (dist <= 0.3f && npc->m_proto->CheckFlag(NPC_ATTACK) && npc->CanAttack())
		{
			// 공격 처리
			// 어택했을경우 AI관련변수 초기화
			npc->InitAIVar();

			if (!npc->m_bStop)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeMoveMsg(rmsg, MSG_MOVE_STOP, npc);
				npc->m_bStop = true;
				SendToCell(rmsg, npc, false);
			}

			//  임시로 테이밍된 npc는 스킬을 사용 못하게 막는다.
			if (/*!MobReadySkill(npc, target_tamingNpc) && */gserver->m_pulse - npc->m_pulseAttack >= npc->m_attackSpeed * 15 / 10 )
			{
				npc->m_pulseAttack = gserver->m_pulse;
				ProcAttack((CCharacter *)npc, (CCharacter *)target_tamingNpc, npc->GetAttackType(NULL), NULL, 0);
			}
		}
	}
	// 타겟이 없으면 테이밍한 캐릭터를 따라 이동
	else if ( owner )
	{
		MobMoveTo(npc, GET_X(owner), GET_Z(owner), true, false, owner);
		return ;
	}
}

void CArea::MobActivity_Confused(CNPC* npc)
{
	CNPC*	target_ConfusedNpc = NULL;	// 혼란 상태의 몬스터가 노리는 몬스터
	CCharacter* temp = NULL;

	// 혼란 상태라면
	int nConfusing = npc->Check_MobFlag( STATE_MONSTER_CONFUSION );
	if ( nConfusing )
	{
		temp = MobSelectTarget_Taming_or_Confused(npc);

		// 몹 스킬 선택//  임시로 혼란 상태의 npc는 스킬을 사용 못하게 막는다.
		//temp = MobSelectSkill(npc, temp);
		if ( temp && IS_NPC(temp) )
		{
			target_ConfusedNpc = TO_NPC(temp);
		}
	}

	if( target_ConfusedNpc && target_ConfusedNpc != npc )
	{
		// 타겟이 있으면 타겟을 향해 이동
		float dist = MobMoveTo(npc, GET_X(target_ConfusedNpc), GET_Z(target_ConfusedNpc), true, true, target_ConfusedNpc);

		if (dist <= 0.3f && npc->m_proto->CheckFlag(NPC_ATTACK) && npc->CanAttack())
		{
			// 공격 처리
			// 어택했을경우 AI관련변수 초기화
			npc->InitAIVar();

			if (!npc->m_bStop)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeMoveMsg(rmsg, MSG_MOVE_STOP, npc);
				npc->m_bStop = true;
				SendToCell(rmsg, npc, false);
			}

			//  임시로 혼란 상태의 npc는 스킬을 사용 못하게 막는다.
			if (/*!MobReadySkill(npc, target_ConfusedNpc) &&*/ gserver->m_pulse - npc->m_pulseAttack >= npc->m_attackSpeed * 15 / 10 )
			{
				npc->m_pulseAttack = gserver->m_pulse;
				ProcAttack((CCharacter *)npc, (CCharacter *)target_ConfusedNpc, npc->GetAttackType(NULL), NULL, 0);
			}
		}
	}

	// 타겟이 없으면 랜덤 이동
	else if ( !target_ConfusedNpc )
	{
		MobMoveRandom(npc);
	}
}

// nCount는 현재 Area에서 npc가 npc를 죽인 수...yhj
void CArea::Monster_synchronization( int nCount )
{
	CNPC* npc = NULL;
	CNPC* tnpc = m_npcList;

	if ( nCount == 0 )
		return;

	if ( tnpc == NULL || tnpc->m_pArea == NULL )
		return;

	// npcList를 검사한다. 단 한명만 삭제하고 루프를 나온다.
	while ((npc = tnpc))
	{
		tnpc = npc->m_pNPCNext;

		if (npc == NULL)
			continue ;

		if ( npc->m_pArea == NULL )
			continue ;

		if ( npc->m_hp == 0 && npc->IsFlag(NPC_LORD_SYMBOL) == 0)
		{
			nCount--;
			// npc->m_pArea-> 이것보다 this-> 이게 안전할 수도... 문제가 되면 바꾸자
			npc->m_pArea->m_nNPC_Killed_NPC = nCount;

			DelAttackList(npc);
			npc->m_pArea->CharFromCell(npc, true);
			npc->m_pArea->DelNPC(npc);
			break;
		}
	} // while ((npc = tnpc))

	// 아직 죽은 npc가 남아 있다면 재귀 함수 호출
	// 동시에 여러명을 삭제하면 NULL 참조가 발생하여 서버 다운 됨...
	if ( nCount > 0 )
	{
		if( tnpc && tnpc->m_pArea )
			tnpc->m_pArea->Monster_synchronization(nCount);
	}
}



void CArea::MobActivity_Mercenary(CNPC* npc)
{
	CPC*	owner = NULL;					// 몬스터를 테이밍한 캐릭터
	CCharacter * target_Attack = NULL;		// 용병 몬스터가 노리는 타겟
	CCharacter * target_Heal = NULL;		// npc가 힐을 해주는 PC(owner)
	CCharacter* temp = NULL;

	owner = npc->GetOwner();

	if( !owner )
		return;

	// 주변에 선태한 타겟이 존재하는지 부터 확인하자.
	if( owner->GetSummonOwners_target() != NULL )
	{
		int i=0;
		for( i=0; i<MSG_CHAR_APET+1 ; i++)
		{
			temp = owner->m_pArea->FindChar( owner->GetSummonOwners_target()->m_index, (__tagMsgCharType)i );
			if( temp )
				break;
		}
	}

	if( temp==NULL )
	{
		owner->SetSummonOwners_target(NULL);
	}


	if (owner == NULL
			|| ( owner && ( owner->m_hp <= 0 || GetDistance((CCharacter*)owner, (CCharacter*)npc) >= 50.0f )) )
	{
		if( owner )
			owner->SummonNpcRemove(npc, true);
		return;
	}

	temp = MobSelectSkill(npc, temp);

	if( temp && IS_PC(temp)  )
	{
		if( npc->GetOwner() == TO_PC(temp) ) // 선택한 자가 오너이면 힐해준다.
			target_Heal = temp;
		else
			target_Attack = temp;
	}
	else
		target_Attack = temp;

	// 힐이면 타겟에게 스킬을 사용한다.
	if ( target_Heal )
	{
		// 스킬은 바로 사용한다.
		MobReadySkill(npc, target_Heal);
		return;
	}

	if( target_Attack && target_Attack != npc)
	{
		// 타겟이 있으면 타겟을 향해 이동
		float dist = MobMoveTo(npc, GET_X(target_Attack), GET_Z(target_Attack), true, true, target_Attack);

		if (dist <= 0.3f && npc->m_proto->CheckFlag(NPC_ATTACK) && npc->CanAttack())
		{
			// 공격 처리
			// 어택했을경우 AI관련변수 초기화
			npc->InitAIVar();

			if (!npc->m_bStop)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeMoveMsg(rmsg, MSG_MOVE_STOP, npc);
				npc->m_bStop = true;
				SendToCell(rmsg, npc, false);
			}

			if ( !MobReadySkill(npc, target_Attack) && gserver->m_pulse - npc->m_pulseAttack >= npc->m_attackSpeed * 15 / 10 )
			{
				npc->m_pulseAttack = gserver->m_pulse;
				ProcAttack((CCharacter *)npc, target_Attack, npc->GetAttackType(NULL), NULL, 0);
			}
		}
	}
	// 타겟이 없으면 테이밍한 캐릭터를 따라 이동
	else if ( owner  && GetDistance((CCharacter*)owner,(CCharacter*)npc) > 3.0f)
	{
		float fRand = GetRandom(0,100)%2 ? 1.0f : -1.0f ;
		float x  = GET_X(owner) + 3.0f * fRand + ((float)(GetRandom( 0 , 200 ) / 100.0f) );
		fRand = GetRandom(0,100)%2 ? 1 : -1 ;
		float z  = GET_Z(owner) + 3.0f * fRand + ((float)(GetRandom( 0 , 200 ) / 100.0f) );

		MobMoveTo(npc, x, z, true, false, owner);
		return ;
	}
}

void CArea::MobActivity_Totem(CNPC *npc)
{
	CPC* owner = npc->GetOwner();

	if (owner == NULL || ( owner && (owner->m_hp <= 0 || GetDistance((CCharacter*)owner, (CCharacter*)npc) >= 100.0f ) ))
	{
		if( owner && owner->GetSummonNpc(npc))
			owner->SummonNpcRemove(npc, true);
		return;
	}

	if( npc->m_currentSkill ) // 스킬을 사용중이면
		return;

	if( npc->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF))
	{
		if ( !(	npc->m_skills[0] && (npc->m_skills[0]->m_proto->m_flag & SF_HELP)
				&& npc->m_skills[0]->IsReady(npc) && npc->CanSpell()))
			return;
	}

	if( npc->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK))
	{
		if ( !(	npc->m_skills[0] && (npc->m_skills[0]->m_proto->m_flag & SF_NOTHELP)
				&& npc->m_skills[0]->IsReady(npc) && npc->CanSpell()))
			return;
	}

	CCharacter * pTarget = NULL;

	pTarget = (CCharacter*)owner;
	if( npc->m_hp > npc->m_skills[0]->m_proto->Level(npc->m_skills[0]->m_level)->m_needHP
			&& npc->m_mp > npc->m_skills[0]->m_proto->Level(npc->m_skills[0]->m_level)->m_needMP
			&& pTarget->CanApplySkill(npc->m_skills[0]->m_proto, npc->m_skills[0]->m_proto->Level(npc->m_skills[0]->m_level)) )
	{
		npc->m_currentSkill = npc->m_skills[0];
		MobReadySkill(npc, npc);
	}
}

void CArea::MobActivity_Suicide(CNPC *npc)
{
	if( npc->m_currentSkill && npc->m_currentSkill->m_state != SKILL_CON_NORMAL )
		return;

	CPC * owner = npc->GetOwner();

	if( !owner || ( owner && ( DEAD(owner) || GetDistance((CCharacter*)owner,(CCharacter*)npc) >= 100.0f ) ) )
	{
		if( owner )
			owner->SummonNpcRemove(npc, true);

		npc->m_hp = 0;
		return;
	}
	float dist = 20.0f;
	CCharacter * target = npc->m_pArea->FindNearestTarget(npc, &dist );

	if( target )
	{
		npc->m_currentSkill = npc->m_skills[0];
		npc->m_currentSkill->m_targetIndex = target->m_index;
		npc->m_currentSkill->m_targetType = target->m_type;
		npc->m_currentSkill->m_state = SKILL_CON_READY;

		dist = GetDistance( (CCharacter*)npc, (CCharacter*)target);
		if( dist > npc->m_skills[0]->m_proto->m_appRange ) // 스킬 유유효기리보다 짧으면 쫒아가라.
		{
			MobMoveTo(npc, GET_X(target), GET_Z(target), true, false, target);
		}
		return;
	}
	else if( owner )// 타겟이 없으면 돌아댕기라~
	{
		float fRand = GetRandom(0,100)%2 ? 1.0f : -1.0f ;
		float x  = GET_X(npc->GetOwner()) + ( fRand * ((float)(GetRandom( 500 , 1000 ) / 100.0f)) );
		fRand = GetRandom(0,100)%2 ? 1.0f : -1.0f ;
		float z  = GET_Z(npc->GetOwner()) + ( fRand * ((float)(GetRandom( 500 , 1000 ) / 100.0f)) );

		MobMoveTo(npc, x, z, true, false, npc->GetOwner());
		return;
	}
}

void CArea::MobActivity_Item_Totem(CNPC *npc)
{
	if(npc == NULL)
		return ;

	CPC* owner = npc->GetOwner();

	if(owner == NULL)
		return ;

	if( owner->m_hp <= 0 || GetDistance((CCharacter*)owner, (CCharacter*)npc) >= 100.0f  )
	{
		if( owner->GetSummonNpc(npc) )
		{
			owner->SummonNpcRemove(npc, true);

			//주인에게 사라짐을 알려준다. (Disappear 패킷을 이용해서)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DisappearMsgForClientEffect(rmsg, npc, owner->m_index);
				SEND_Q(rmsg, owner->m_desc);
			}
		}
		return;
	}

	if( npc->m_currentSkill ) // 스킬을 사용중이면
		return;

	////
	CCharacter * pTarget = NULL;
	pTarget = (CCharacter*)owner;

	int skillindex = -1;
	int i;
	
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		int skillprob = GetRandom(1, 10000);
		// 스킬 사용 가능 검사
		if (npc->m_skills[i]
		&& npc->m_proto->m_skillProb[i] >= skillprob
			&& npc->m_skills[i]->IsReady(npc)
			&& npc->CanSpell()
			&& npc->m_hp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needHP
			&& npc->m_mp > npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)->m_needMP
			&& pTarget->CanApplySkill(npc->m_skills[i]->m_proto, npc->m_skills[i]->m_proto->Level(npc->m_skills[i]->m_level)))
		{
			skillindex = i;
			break;
		}
	}
	
	if (skillindex >= 0)
	{
		npc->m_currentSkill = npc->m_skills[skillindex];
		/*npc->m_currentSkill->m_targetType = pTarget->m_type;
		npc->m_currentSkill->m_targetIndex = pTarget->m_index;*/
		MobReadySkill(npc, npc);
	}
}

/*
bool MobProcSkillMercenary(CNPC *npc)
{
	// 현재 스킬 사용중인가
	if (!npc->m_currentSkill )
		return false;

	if( !npc->GetOwner() )
		return false;

	CNetMsg::SP rmsg(new CNetMsg);
	bool bSkip = false;
}
*/

bool MobProcSkillTotem_Buff(CNPC *npc)
{
	// 현재 스킬 사용중인가
	if (!npc->m_currentSkill )
		return false;

	if( !npc->GetOwner() )
		return false;

	bool bSkip = false;

	switch( npc->m_currentSkill->m_state )
	{
	case SKILL_CON_READY:
		try
		{
			// 스킬 레디->파이어 검사
			if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime <= gserver->m_pulse)
			{
				// hp, mp 검사
				if (npc->m_hp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needHP
						|| npc->m_mp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needMP)
					throw 0;

				CPC* owner = npc->GetOwner();
				int	mcount = npc->m_currentSkill->m_proto->m_levelproto[npc->m_currentSkill->m_level - 1]->m_targetNum ;

				boost::scoped_array<MultiTarget> multitarget(new MultiTarget[mcount]);

				bool bApply;
				int i;

				if( owner->m_party == NULL && owner->m_Exped == NULL)
				{
					mcount = 0;
					float dist = GetDistance((CCharacter*)npc, (CCharacter*)owner);
					if( dist <= npc->m_currentSkill->m_proto->m_appRange )
					{
						multitarget[mcount].mtarget			= (CCharacter*)owner;
						multitarget[mcount].mdist			= dist;
						multitarget[mcount].mtargettype		= owner->m_type;
						multitarget[mcount].mtargetindex	= owner->m_index;
						mcount++;
					}

					// 팻도 주자
					if( owner->GetAPet() )
					{
						dist = GetDistance((CCharacter*)npc, (CCharacter*)owner->GetAPet());
						if( dist <= npc->m_currentSkill->m_proto->m_appRange )
						{
							multitarget[mcount].mtarget			= (CCharacter*)owner->GetAPet();
							multitarget[mcount].mdist			= dist;
							multitarget[mcount].mtargettype		= owner->GetAPet()->m_type;
							multitarget[mcount].mtargetindex	= owner->GetAPet()->m_index;
							mcount++;
						}
					}

					// 소환주수도 주자
					if( owner->m_job == JOB_SORCERER )
					{
						CElemental* elemental = owner->m_elementalList;
						while (elemental)
						{
							dist = GetDistance((CCharacter*)npc, (CCharacter*)elemental);
							if( dist <= npc->m_currentSkill->m_proto->m_appRange )
							{
								multitarget[mcount].mtarget			= (CCharacter*)elemental;
								multitarget[mcount].mdist			= dist;
								multitarget[mcount].mtargettype		= elemental->m_type;
								multitarget[mcount].mtargetindex	= elemental->m_index;
								mcount++;
							}
							elemental = elemental->m_nextElemental;
						}
					}
				}
				else if( owner->m_party || owner->m_Exped )// 파티원에게 모두 버프를 뿌린다.
				{
					int realCount = 0;
					int nCount;
					int n, j;
					int group = -1;

					if( owner->m_party )
					{
						nCount = MAX_PARTY_MEMBER;
						group = -1;
					}

					if( owner->m_Exped )
					{
						nCount = MAX_EXPED_GMEMBER;
						group = owner->m_Exped->FindMemberGroupIndex(owner->m_index);
						if( group == -1 )
							return true;
					}

					for (n = 0; n < nCount ; n++)
					{
						const CPartyMember* pPartyMember = NULL;
						CExpedMember * pExpedMember = NULL;
						CPC * pPC = NULL;

						if( owner->m_party  )
						{
							pPartyMember = owner->m_party->GetMemberByListIndex(n);
							if( !pPartyMember )
								continue;
							pPC = pPartyMember->GetMemberPCPtr();
						}

						if( owner->m_Exped )
						{
							pExpedMember = owner->m_Exped->GetMemberByListIndex(group, n);
							if( !pExpedMember )
								continue;
							pPC = pExpedMember->GetMemberPCPtr();
						}

						if( pPC )
						{
							float dist = GetDistance((CCharacter*)npc, (CCharacter*)pPC);
							if( dist <= npc->m_currentSkill->m_proto->m_appRange )
							{
								for(i=0; i<mcount; i++)
								{
									if( multitarget[i].mdist >= dist )
									{
										for(j=mcount-1; j>i; j--)
										{
											multitarget[j].mtarget		= multitarget[j-1].mtarget;
											multitarget[j].mtargetindex	= multitarget[j-1].mtargetindex;
											multitarget[j].mtargettype	= multitarget[j-1].mtargettype;
											multitarget[j].mdist		= multitarget[j-1].mdist;
										}
										multitarget[i].mtarget		= (CCharacter*)pPC;
										multitarget[i].mtargetindex	= pPC->m_index;
										multitarget[i].mtargettype	= pPC->m_type;
										multitarget[i].mdist		= dist;
										realCount ++;
										break;
									}
								}
							}
							// 팻도 주자
							if( pPC->GetAPet() )
							{
								dist = GetDistance((CCharacter*)npc, (CCharacter*)pPC->GetAPet());
								if( dist <= npc->m_currentSkill->m_proto->m_appRange )
								{
									for(i=0; i<mcount; i++)
									{
										if( multitarget[i].mdist >= dist )
										{
											for(j=mcount-1; j>i; j--)
											{
												multitarget[j].mtarget		= multitarget[j-1].mtarget;
												multitarget[j].mtargetindex	= multitarget[j-1].mtargetindex;
												multitarget[j].mtargettype	= multitarget[j-1].mtargettype;
												multitarget[j].mdist		= multitarget[j-1].mdist;
											}
											multitarget[i].mtarget		= (CCharacter*)pPC->GetAPet();
											multitarget[i].mtargetindex	= pPC->GetAPet()->m_index;
											multitarget[i].mtargettype	= pPC->GetAPet()->m_type;
											multitarget[i].mdist		= dist;
											realCount ++;
											break;
										}
									}
								}
							}

							// 소환주수도 주자
							if( pPC->m_job == JOB_SORCERER )
							{
								CElemental* elemental = pPC->m_elementalList;
								while (elemental)
								{
									dist = GetDistance((CCharacter*)npc,(CCharacter*)elemental);
									if( dist <= npc->m_currentSkill->m_proto->m_appRange )
									{
										for(i=0; i<mcount; i++)
										{
											if( multitarget[i].mdist >= dist )
											{
												for(j=mcount-1; j>i; j--)
												{
													multitarget[j].mtarget		= multitarget[j-1].mtarget;
													multitarget[j].mtargetindex	= multitarget[j-1].mtargetindex;
													multitarget[j].mtargettype	= multitarget[j-1].mtargettype;
													multitarget[j].mdist		= multitarget[j-1].mdist;
												}
												multitarget[i].mtarget		= (CCharacter*)elemental;
												multitarget[i].mtargetindex	= elemental->m_index;
												multitarget[i].mtargettype	= elemental->m_type;
												multitarget[i].mdist		= dist;
												realCount ++;
												break;
											}
										}
									}
									elemental = elemental->m_nextElemental;
								}
							}
						}
					}
					if( realCount <= mcount )
						mcount = realCount;
				}
				else
					return true;

				if( mcount > 0)
				{
					npc->m_currentSkill->Fire();

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillFireMsg(rmsg, npc, npc->m_currentSkill, (CCharacter*)owner, mcount, multitarget.get(), 0, 0.0f, 0.0f, 0.0f, 0.0f, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					// 스킬 적용.
					for(i=0; i<mcount; i++)
					{
						ApplySkill(npc, multitarget[i].mtarget, npc->m_currentSkill, -1, bApply);
					}
				}
				break;
			}
		}
		catch (...)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill = NULL;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillCancelMsg(rmsg, npc);
				npc->m_pArea->SendToCell(rmsg, npc, false);
			}
		}
		bSkip = true;
		break;

	case SKILL_CON_FIRE:
		if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime + npc->m_currentSkill->m_proto->m_fireTime <= gserver->m_pulse)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill = NULL;
			npc->m_aipulse = gserver->m_pulse;
			npc->m_delay = NPC_ACTIVITY_DELAY;
		}
		else
			bSkip = true;

		break;
	}

	return bSkip;
}

bool MobProcSkillTrap_Attk(CNPC *npc)
{
	if (npc->GetOwner() == NULL		// 주인 없다.
			|| (npc->GetOwner()
				&& ( npc->GetOwner()->m_hp <= 0
					 || GetDistance((CCharacter*)npc->GetOwner(), (CCharacter*)npc) >= 100.0f
				   )
			   )
	   )
	{
		if( npc->GetOwner() )
			npc->GetOwner()->SummonNpcRemove(npc);
		return true;
	}

	if( !npc->m_currentSkill ) // 강제 세팅
	{
		npc->m_currentSkill = npc->m_skills[0];
		npc->m_currentSkill->m_targetIndex = npc->m_index;
		npc->m_currentSkill->m_targetType = npc->m_type;
		npc->m_currentSkill->m_state = SKILL_CON_READY;
	}

	switch( npc->m_currentSkill->m_state )
	{
	case SKILL_CON_NORMAL:
	case SKILL_CON_READY:
		try
		{
			// 스킬 레디->파이어 검사
			if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime <= gserver->m_pulse)
			{
				// hp, mp 검사
				if (npc->m_hp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needHP
						|| npc->m_mp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needMP)
					throw 0;

				int	mcount = npc->m_currentSkill->m_proto->m_levelproto[npc->m_currentSkill->m_level - 1]->m_targetNum ; // 공격 가능수는 10명
				boost::scoped_array<MultiTarget> multitarget(new MultiTarget[mcount]);
				mcount = GetSummonNpcMultiTarget(npc, multitarget.get(), mcount);

				if( mcount > 0)
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillReadyMsg(rmsg, npc, npc->m_currentSkill, npc);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					{
						npc->m_currentSkill->Fire();
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillFireMsg(rmsg, npc, npc->m_currentSkill, (CCharacter*)npc, mcount, multitarget.get(), 0, 0.0f, 0.0f, 0.0f, 0.0f, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					// 스킬 적용
					bool bApply = false;
					for(int i = 0; i < mcount; i++)
					{
						if (multitarget[i].mtarget == NULL)
							continue;

						if (multitarget[i].mtarget->m_index <= 0)
							continue;

						ApplySkill(npc, multitarget[i].mtarget, npc->m_currentSkill, -1, bApply);
					}
					npc->GetOwner()->SummonNpcRemove(npc);

					return true;
				}
			}
		}
		catch (...)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill = NULL;
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillCancelMsg(rmsg, npc);
			npc->m_pArea->SendToCell(rmsg, npc, false);
		}
		break;

	case SKILL_CON_FIRE:
		if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime + npc->m_currentSkill->m_proto->m_fireTime <= gserver->m_pulse)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill = NULL;
			npc->m_aipulse = gserver->m_pulse;
			npc->m_delay = NPC_ACTIVITY_DELAY;
		}
		return true;
	}
	return true;
}

bool MobProcSkillSuicide_Attk(CNPC *npc)
{
	if( !npc->m_currentSkill )
		return false;

	if( npc->m_currentSkill->m_state == SKILL_CON_NORMAL  )
		return false;

	bool bSkip = false;
	switch( npc->m_currentSkill->m_state )
	{
	case SKILL_CON_READY:
		try
		{
			// 스킬 레디->파이어 검사
			if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime <= gserver->m_pulse)
			{
				// hp, mp 검사
				if (npc->m_hp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needHP
						|| npc->m_mp <= npc->m_currentSkill->m_proto->Level(npc->m_currentSkill->m_level)->m_needMP)
					throw 0;

				int	mcount = npc->m_currentSkill->m_proto->m_levelproto[npc->m_currentSkill->m_level - 1]->m_targetNum ; // 공격 가능수는 10명
				boost::scoped_array<MultiTarget> multitarget(new MultiTarget[mcount]);
				mcount = GetSummonNpcMultiTarget(npc, multitarget.get(), mcount);

				if( mcount > 0)
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillReadyMsg(rmsg, npc, npc->m_currentSkill, npc);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					npc->m_currentSkill->Fire();

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillFireMsg(rmsg, npc, npc->m_currentSkill, (CCharacter*)npc, mcount, multitarget.get(), 0, 0.0f, 0.0f, 0.0f, 0.0f, 0);
						npc->m_pArea->SendToCell(rmsg, npc, false);
					}

					// 스킬 적용.
					bool bApply = false;
					int i;
					for(i=0; i<mcount; i++)
					{
						if( multitarget[i].mtarget )
							ApplySkill(npc, multitarget[i].mtarget, npc->m_currentSkill, -1, bApply);
					}
					npc->GetOwner()->SummonNpcRemove(npc);
					bSkip = true;
				}
				else
				{
					npc->m_currentSkill->Ready(NULL);
					npc->m_currentSkill->m_state = SKILL_CON_NORMAL;
					npc->m_currentSkill = NULL;
				}
			}
		}
		catch (...)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill->m_state = SKILL_CON_NORMAL;
			npc->m_currentSkill = NULL;
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillCancelMsg(rmsg, npc);
			npc->m_pArea->SendToCell(rmsg, npc, false);
		}
		break;

	case SKILL_CON_FIRE:
		if (npc->m_currentSkill->m_usetime + npc->m_currentSkill->m_proto->m_readyTime + npc->m_currentSkill->m_proto->m_stillTime + npc->m_currentSkill->m_proto->m_fireTime <= gserver->m_pulse)
		{
			npc->m_currentSkill->Cancel(npc);
			npc->m_currentSkill = NULL;
			npc->m_aipulse = gserver->m_pulse;
			npc->m_delay = NPC_ACTIVITY_DELAY;
		}
		bSkip =  true;
		break;
	}
	return bSkip;
}

bool MobProcSkillItemTotem_Buff(CNPC *npc)
{
	return true;
}

int GetSummonNpcMultiTarget(CNPC* npc, MultiTarget *multitarget, int mcount)
{
	if( !npc->GetOwner())
		return 0;

	int x, z;
	int sx = npc->m_cellX - CELL_EXT;
	int ex = npc->m_cellX + CELL_EXT;
	int sz = npc->m_cellZ - CELL_EXT;
	int ez = npc->m_cellZ + CELL_EXT;

	CPC* owner = npc->GetOwner();
	int realCount = 0;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= npc->m_pArea->m_size[0]) continue;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= npc->m_pArea->m_size[1]) continue;

			CCharacter *pChar;
			CCharacter *pCharNext = npc->m_pArea->m_cell[x][z].m_listChar;

			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if ( pChar == NULL || DEAD(pChar) )
					break;

				if( pChar->m_index < 0)
					continue;

				CPC *ptowner = NULL;
				if( IS_PC(pChar) )
					ptowner = TO_PC(pChar);
				else if( IS_PET(pChar) )
					ptowner = TO_PET(pChar)->GetOwner();
				else if( IS_APET(pChar) )
				{
					if( ! TO_APET(pChar)->IsProtoFlag( APET_FLAG_ATTACK ) )
						continue;
					ptowner = TO_APET(pChar)->GetOwner();
				}
				else if( IS_ELEMENTAL(pChar) )
					ptowner = TO_ELEMENTAL(pChar)->GetOwner();
				else if( IS_NPC(pChar) )
				{
					if( npc == TO_NPC(pChar))
						continue;

					if( TO_NPC(pChar)->m_proto->CheckFlag(NPC_SHOPPER | NPC_ZONEMOVER | NPC_REFINER | NPC_QUEST | NPC_PEACEFUL | NPC_MINERAL | NPC_CROPS | NPC_ENERGY | NPC_REMISSION) )
						continue;

					if( TO_NPC(pChar)->GetOwner() && TO_NPC(pChar)->GetOwner()->GetSummonNpc(TO_NPC(pChar)) )
						ptowner = TO_NPC(pChar)->GetOwner();
				}

				if( ptowner && ptowner ==  owner)
					continue;

				bool bTarget = false;

				float tdist = GetDistance((CCharacter*)npc, pChar);
				// 공격 가능한가 상태인가?
				if( pChar != npc && !DEAD(pChar) && pChar != (CCharacter*)owner
						&& ABS(GET_YLAYER(pChar) - GET_YLAYER(npc)) <= 1
						&& tdist <= npc->m_currentSkill->m_proto->m_appRange )
				{
					int zoneindex = owner->m_pZone->m_index;
					CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);

					if( castle != NULL && castle->GetState() != WCSF_NORMAL // 공성 중이고
							&& owner->GetMapAttr() & MATT_WAR  // 공성 지역이고
							&& ( IS_ATTACK_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) )
								 || IS_DEFENSE_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) ) ) ) // 오너가 공성에 참여해 있다면
					{
						if( IS_ATTACK_TEAM(owner->GetJoinFlag( owner->m_pZone->m_index) ) )// 공성 측이라면
						{
							if( ptowner)
							{
								if( IS_DEFENSE_TEAM( ptowner->GetJoinFlag(owner->m_pZone->m_index))  )
									bTarget =true;
							}
							else if( IS_NPC(pChar))
							{
								if( !TO_NPC(pChar)->m_proto->CheckFlag(NPC_LORD_SYMBOL) && TO_NPC(pChar)->m_proto->CheckFlag(NPC_WARCASTLE) )
									bTarget = true;
							}
						}
						else if(IS_DEFENSE_TEAM( owner->GetJoinFlag(owner->m_pZone->m_index) ) ) // 수성 측이라면
						{
							if( ptowner )
							{
								if( IS_ATTACK_TEAM( ptowner->GetJoinFlag(owner->m_pZone->m_index) ) )
								{
									if( IS_NPC(pChar)
											&& (TO_NPC(pChar)->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF) ||TO_NPC(pChar)->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK)) )
										continue;

									bTarget =true;
								}
							}
						}
						else if( ptowner && ( IsRaList(owner, ptowner) || IsRaList(ptowner, owner) ))
							bTarget = true;
						else if( ptowner && owner->IsSetPlayerState( PLAYER_STATE_PKMODE) && ptowner->IsSetPlayerState(PLAYER_STATE_PKMODE ))
							bTarget = true;
					}
					else if( ptowner && ( IsRaList(owner, ptowner) || IsRaList(ptowner, owner) ))
					{
						if( IS_NPC(pChar)
								&& (TO_NPC(pChar)->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF) ||TO_NPC(pChar)->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK) ))
							continue;

						bTarget = true;
					}
					else if( ptowner && owner->IsSetPlayerState( PLAYER_STATE_PKMODE) && ptowner->IsSetPlayerState(PLAYER_STATE_PKMODE ))
						bTarget = true;
					else if (IS_NPC(pChar) && !ptowner)
					{
						if( TO_NPC(pChar)->m_proto->CheckFlag(NPC_CASTLE_TOWER | NPC_GUARD | NPC_CASTLE_GUARD)
								|| TO_NPC(pChar)->m_idNum==491
						  ) // 몬스터 콤보존의 바닥 데이미지는 빼자
							continue;
						bTarget = true;
					}

					if( bTarget )
					{
						int i,j;

						for(i=0; i<mcount; i++)
						{
							if( multitarget[i].mdist >= tdist )
							{
								for(j=mcount-1; j>i; j--)
								{
									multitarget[j].mtarget		= multitarget[j-1].mtarget;
									multitarget[j].mtargetindex	= multitarget[j-1].mtargetindex;
									multitarget[j].mtargettype	= multitarget[j-1].mtargettype;
									multitarget[j].mdist		= multitarget[j-1].mdist;
								}
								multitarget[i].mtarget		= pChar;
								multitarget[i].mtargetindex	= pChar->m_index;
								multitarget[i].mtargettype	= pChar->m_type;
								multitarget[i].mdist		= tdist;
								realCount ++;
								break;
							}
						}
					}
				}
			}
		}
	}

	if( mcount < realCount )
		realCount = mcount;

	return realCount;
}
