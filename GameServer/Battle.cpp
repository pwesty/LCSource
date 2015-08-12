#include "stdhdrs.h"

#include "CmdMsg.h"
#include "Server.h"
#include "Exp.h"
#include "Log.h"
#include "Battle.h"
#include "hardcoding.h"
#include "WarCastle.h"
#include "doFunc.h"
#include "HolyWaterData.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

///////////////////
// 공격 관련 함수들

int ProcAttack(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, CSkill* skill, int magicindex, int damage)
{
	// 방어자가 공격 당할 수 있는 상태인지 검사
	if (!IsAvailableAttack(of, df))
		return 0;

	// 공격 종류 구분
	if (damageType == MSG_DAMAGE_UNKNOWN)
		return 0;

	const CSkillProto* proto = NULL;
	const CSkillLevelProto* levelproto = NULL;
	const CMagicProto* magic = NULL;
	const CMagicLevelProto* magiclevel = NULL;

	if (skill)
	{
		proto = skill->m_proto;
		if (!GetSkillPrototypes(proto, skill->m_level, magicindex, &levelproto, &magic, &magiclevel))
			return 0;
	}

	// 방어자 공격에 따른 버프 취소
	df->m_assist.CancelSleep();
	df->m_assist.CancelFear();
	df->CancelInvisible();
	df->m_assist.CancelMantle();
	of->m_assist.CancelMantle();

	if( IS_APET(df) )
	{
		CAPet* apet = TO_APET(df);
		if ( apet && apet->GetOwner() && apet->IsMount() )
		{
			df = (CCharacter*) apet->GetOwner() ;
			if(DEAD(df))
				return 0;
		}
	}

	// 공격자 공격에 따른 버프 취소
	of->CancelInvisible();
	if ( IS_APET(of) )
	{
		if(TO_APET(of)->GetOwner())
			TO_APET(of)->GetOwner()->CancelInvisible();
	}

	char flag;
	bool isCharge = false;
	switch (damageType)
	{
	case MSG_DAMAGE_REFLEX:
	case MSG_DAMAGE_LINK:
		flag = HITTYPE_NORMAL;
		if (damage == 0)
			return 0;
		break;

	default:
		// 명중/회피/크리티컬 구분 플래그
		flag = GetHitType(of, df, damageType);
		// 명중/회피/크리티컬 검사
		if (flag != HITTYPE_MISS)
			flag = SelectHitType(of, df, damageType, flag, magic, magiclevel, true);
		// 데미지 계산		
		damage = GetDamage(of, df, damageType, flag, magic, magiclevel, isCharge);
		break;
	}

	// 자살 공격은 NPC를 제와하고는 데미지를 10%만 적용
	if( proto && proto->m_index == 1110 && !IS_NPC(df) )
		damage = (int)(damage * 0.1f);

	ApplyHateByDamage(of, df, flag, damage);
	ApplyDamage(of, df, damageType, proto, magic, damage, flag, isCharge);

	//보석 옵션 체크하여 스킬 발동 처리
	//옵션 스킬 발동
	if(IS_PC(of))
	{
		CPC* pc = TO_PC(of);
		if(pc->m_optionAttSkillList.count() > 0)
		{
			int optionCount  = pc->m_optionAttSkillList.count() > 0;
			void* pos = pc->m_optionAttSkillList.GetHeadPosition();
			bool bApply;
			for(int i=0; i<optionCount; i++)
			{
				int rand = GetRandom(1, 10000);
				CSkill *skill = pc->m_optionAttSkillList.GetNext(pos);
				if(rand < skill->m_optionSkillProb)
				{
					ApplySkill(of, df, skill,-1, bApply);
				}
			}
		}

#ifdef DURABILITY
		// 무기 장비 내구도 감소 (공격자 이므로)
		pc->calcDurability_for_weapon();
#endif
	}
	if(IS_PC(df))
	{
		CPC* pc = TO_PC(df);
		if(pc->m_optionDefSkillList.count() > 0)
		{
			bool bRet;
			int optionCount  = pc->m_optionDefSkillList.count();
			void* pos = pc->m_optionDefSkillList.GetHeadPosition();
			bool bApply;
			bool bHits[] = {true, true, true};
			for(int i=0; i<optionCount; i++)
			{
				CSkill *skill = pc->m_optionDefSkillList.GetNext(pos);
				int rand = GetRandom(1, 10000);
				if(rand < skill->m_optionSkillProb)
				{
					if (df->m_assist.Add(df, -1, skill->m_proto, skill->m_level, bHits, true, -1,
										 -1,
										 0, 0, 0))
						bRet = true;
					ApplySkill(df, df, skill,-1, bApply);
				}
			}
		}

#ifdef DURABILITY
		// 방어구(액세서리) 장비 내구도 감소 (방어자 이므로)
		pc->calcDurability_for_armor();
#endif
	}

	if (flag != HITTYPE_MISS)
		ProcAfterHit(of, df);

	if (DEAD(df))
	{
		switch (df->m_type)
		{
		case MSG_CHAR_PC:
			ProcDead(TO_PC(df), of);
			break;
		case MSG_CHAR_NPC:
			ProcDead(TO_NPC(df), of);
			break;
		case MSG_CHAR_PET:
			ProcDead(TO_PET(df), of);
			break;
		case MSG_CHAR_ELEMENTAL:
			ProcDead(TO_ELEMENTAL(df), of);
			break;
		case MSG_CHAR_APET:
			ProcDead(TO_APET(df), of );
			break;
		default:
			break;
		}

		// 여기 오면 df가 메모리에서 해재 될 수 있다
		// 그래서 아예 NULL로 바꿔버린다
		df = NULL;

		return 1;
	}
	else
	{
		// 살아 남으면 데미지 반사를 검사
		if (damageType != MSG_DAMAGE_REFLEX && GetDistance(of, df) <= 25)
		{
			int reflexdam = 0;
			if (GetRandom(1, 10000) <= df->m_avPassiveAddition.hcReflexProb)
			{
				reflexdam += df->m_avPassiveAddition.hcReflex;
				reflexdam += damage * df->m_avPassiveRate.hcReflex / SKILL_RATE_UNIT;
			}
			if (GetRandom(1, 10000) <= df->m_assist.m_avAddition.hcReflexProb)
			{
				reflexdam += df->m_assist.m_avAddition.hcReflex;
				reflexdam += damage * df->m_assist.m_avRate.hcReflex / SKILL_RATE_UNIT;
			}
			if (reflexdam)
			{
				if (ProcAttack(df, of, MSG_DAMAGE_REFLEX, NULL, 0, reflexdam) == 1)
					return -1;
			}
		}

		if ( IS_PC(of) && IS_NPC(df) )
		{
			// 테이밍 몬스터는 주인이 때리는 타겟만 때려야 함
			// 여기서 pc가 때리는 npc 저장
			CPC* temp = TO_PC(of);
			if(temp)
				temp->SetOwners_target(df);
		}

		if( IS_PC(of) )
		{
			CPC* pPcOf = TO_PC(of);
			if(pPcOf)
				pPcOf->SetSummonOwners_target(df);

			if( IS_NPC(df) )
			{
				CNPC * pNpc = TO_NPC(df);
				if( pNpc->GetOwner() && pNpc->GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == pNpc)
					pNpc->GetOwner()->SetSummonOwners_target(of);
			}

			if( pPcOf->IsSetPlayerState(PLAYER_STATE_PKMODE)  )
			{
				if(  IS_PC( df) )
				{
					CPC* pPcDf = TO_PC(df);
					if(pPcDf && pPcDf->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) )
						pPcDf->SetSummonOwners_target(of);
				}
				// PVP 상태에서 용병을 때리면 맞은 용병의 오너에게 정당 방위를 성립 시킨다.
				else if( IS_NPC(df) && TO_NPC(df)->GetOwner() && TO_NPC(df) == TO_NPC(df)->GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) )
				{
					CPC* pPcDf = TO_NPC(df)->GetOwner();
					AddRaList(pPcOf, pPcDf ); //
				}
			}
		}

	}

	return 0;
}

// ch, tch가 CELL_EXT 범위 안에 있는가 있으면 true
bool CheckInNearCellExt(CCharacter* ch, CCharacter* tch)
{
	// 디버깅용
	int chType = ch->m_type;
	int tchType = tch->m_type;
	chType = chType;
	tchType = tchType;
	if (ch == NULL)
		return false;
	if (tch == NULL)
		return false;
	if (ch->m_pZone == NULL)
		return false;
	if (tch->m_pZone == NULL)
		return false;
	if (ch->m_pArea == NULL)
		return false;
	if (ch->m_pZone == NULL)
		return false;
	if (ch->m_pZone->m_index != tch->m_pZone->m_index)
		return false;
	if (ch->m_pArea->m_index != tch->m_pArea->m_index)
		return false;

	if (ch->m_pos.m_yLayer != tch->m_pos.m_yLayer)
		return false;

	if (ABS(ch->m_cellX - tch->m_cellX) <= CELL_EXT && ABS(ch->m_cellZ - tch->m_cellZ) <= CELL_EXT)
		return true;

	return false;
}

// 우선권 PC 찾기
// target list들의 평균 레벨 구하기
// 찾을때 사용할 데이터
typedef struct __tagPreferencePCData
{
	CPC*		m_pc;		// PC 포인터
	int			m_damage;	// 해당 PC의 데미지
} PPDATA;

CPC* FindPreferencePC(CNPC* npc, int* level, LONGLONG* pnTotalDamage)
{
	CPC* ret = NULL;
	CAttackChar* target = npc->m_attackList;

	int dam = 0;
	*pnTotalDamage = npc->GetTotalDamage();
	LONGLONG nPreferDamage = *pnTotalDamage + npc->m_maxHP * 2 / 10;		// 최대 대미지와 NPC의 최대HP 20%를 합한 값을 아이템 우선권에 사용
	int totLevel = 0;
	int	count = 0;

	std::map<CPC*, PPDATA> listAttackPC;	// NPC 공격에 가담한 PC 리스트
	while (target)
	{
		if (target->ch)
		{
			// 디버깅용
			int npcType = npc->m_type;
			int tchType = target->ch->m_type;
			npcType = npcType;
			tchType = tchType;
			if (npc== NULL)
				break;
			if (target->ch == NULL)
				break;
			if (npc->m_pZone == NULL)
				break;
			if (target->ch->m_pZone == NULL)
				break;
			if (target->ch->m_pArea == NULL)
				break;
			if (npc->m_pZone == NULL)
				break;
			// 같은 zone에 없거나 같은 area에 없거나 CELL_EXT(+-5) 범위 안에 없거나
			if ((npc->m_pZone->m_index != target->ch->m_pZone->m_index) ||
					(npc->m_pArea->m_index != target->ch->m_pArea->m_index) ||
					!CheckInNearCellExt(npc, target->ch))
				break;

			dam = target->m_damage;

			// 선공 pc는 NPC 최대 HP의  20% 보너스!!
			if (target->m_next == NULL)
				dam += npc->m_maxHP * 2 / 10;

			PPDATA ppdata = {NULL, 0};
			switch (target->ch->m_type)
			{
			case MSG_CHAR_PC:
				ppdata.m_pc = target->pc;
				break;

			case MSG_CHAR_PET:
				if (TO_PET(target->ch)->GetOwner())
					ppdata.m_pc = TO_PET(target->ch)->GetOwner();
				break;

			case MSG_CHAR_ELEMENTAL:
				if (TO_ELEMENTAL(target->ch)->GetOwner())
					ppdata.m_pc = TO_ELEMENTAL(target->ch)->GetOwner();
				break;

			case MSG_CHAR_APET:
				if ( TO_APET(target->ch)->GetOwner() )
					ppdata.m_pc = TO_APET(target->ch)->GetOwner();
				break;

			case MSG_CHAR_NPC:
				if ( TO_NPC(target->ch)->GetOwner() )
					ppdata.m_pc = TO_NPC(target->ch)->GetOwner();
				break;
			default:
				break;
			}

			if (ppdata.m_pc)
			{
				std::map<CPC*, PPDATA>::iterator it = listAttackPC.find(ppdata.m_pc);
				if (it == listAttackPC.end())
				{
					PPDATA ppdatanew;
					ppdatanew.m_pc = ppdata.m_pc;
					ppdatanew.m_damage = dam;
					listAttackPC.insert(std::map<CPC*, PPDATA>::value_type(ppdatanew.m_pc, ppdatanew));
					count++;
					totLevel += ppdata.m_pc->m_level;
				}
				else
				{
					PPDATA& ppdatacur = it->second;
					ppdatacur.m_damage += dam;
				}
			}

			if( target->ch->m_type == MSG_CHAR_NPC )
			{
				if( TO_NPC(target->ch)->GetOwner() && TO_NPC(target->ch)->GetOwner()->GetSummonNpc(TO_NPC(target->ch)) )
				{
					target = target->m_next;
					continue;
				}
			}
		}
		target = target->m_next;
	}

	std::map<CPC*, PPDATA>::iterator it = listAttackPC.begin();
	std::map<CPC*, PPDATA>::iterator endit = listAttackPC.end();
	for(; it != endit; ++it)
	{
		PPDATA& ppdata = it->second;

		// 50% 이상 데미지 준 PC 반환
		if (ppdata.m_damage > nPreferDamage / 2)
			ret = ppdata.m_pc;
	}

	listAttackPC.clear();

	if (count == 0)
		*level = -1;
	else
	{
		*level = totLevel / count;
		if (*level < 0)
			*level = MAX_LEVEL;
	}

	return ret;
}

// 균등 파티 시 돈 갈라먹기
void DivisionPartyMoney(CPC* pc, LONGLONG count)
{
	// validation
	if (pc == NULL || count <= 0)
		return;

	int i;
	CParty* party = pc->m_party;
	int divCount = 0;				// 돈 갈라먹을 파티 멤버 수

	for (i=0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMemberPC = party->GetNearMember(pc, i);
		if (!pMemberPC)
			continue;

		divCount++;
	}

	if (divCount < 1)
		return;

	// 돈 갈라내기
	//bool bTake = false;		// 돈을 받은 사람이 있나?: 없으면 아이템은 바닥에 그대로
	count = count / divCount;

	if (count < 1)
		return;

	GoldType_t Artimoney = 0;

	// 파티원에게 분배
	for (i = MAX_PARTY_MEMBER - 1; i >= 0; i--)
	{
		Artimoney = 0;

		CPC* pMemberPC = TO_PC(party->GetNearMember(pc, i));
		if (!pMemberPC)
			continue;

		// 갈른 돈 만들기
		int bonus = 0;
		if(pMemberPC->m_avPassiveAddition.money_nas > 0)
		{
			bonus += pMemberPC->m_avPassiveAddition.money_nas;	
		}
		if(pMemberPC->m_avPassiveRate.money_nas > 0)
		{
			bonus = count * (pMemberPC->m_avPassiveRate.money_nas - 100) / SKILL_RATE_UNIT;
		}

		Artimoney = count * pMemberPC->m_artiGold / 100;

		pMemberPC->m_inventory.increaseMoney(count + Artimoney, bonus);
	}
}

// 균등파티일때 아이템 랜덤으로 가져가지
bool GetItemRandomParty(CPC* pc, CItem* item)
{
	// validation
	if (!pc && !item && !pc->m_party)
		return false;

	int i;
	CParty* party = pc->m_party;
	bool bPreference = false;		// 이 파티가 자격이 있는가
	int partyCount = 0;				// 받을 수 있는 자격이 있는 파티원 수

	for (i=0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMemberPC = party->GetNearMember(pc, i);
		if (!pMemberPC)
			continue;

		if (item->m_preferenceIndex == pMemberPC->m_index)
			bPreference = true;

		partyCount++;
	}

	// 우선권이 있는데 파티가 자격없으면 return;
	if (item->m_preferenceIndex != -1 && !bPreference)
	{
		return false;
	}

	// 받을 사람이 없다.
	if (partyCount < 1)
		return false;

	bool bGiveItem = false;		// 아이템 가져 갔다!
	int idx;					// 결정된 파티원 인덱스

	int bitfield = 0;			// 결정됬으나 실패한 파티원 인덱스 저장
	bool bAll = false;

	int deleteVIndex = item->getVIndex();

	std::vector<CPC*> _user;

	for(int i = 0 ; i < party->GetMemberCount(); i++)
	{
		CPC* party_member = party->GetNearMember(pc, i);
		
		if(party_member == NULL)
			continue;

		else if(party_member->m_inventory.getEmptyCount() == 0)
			continue;

		else if(gserver->isActiveEvent(A_EVENT_EGGS_HUNT) &&
			item->getDBIndex() == 2148 &&
			party_member->m_inventory.FindByDBIndex(item->getDBIndex()) != NULL)
			continue;

		else if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
		{
			if(party_member->m_level <= PKMODE_LIMIT_LEVEL)
				continue;
			if(party_member->m_assist.FindBySkillIndex(PVP_PROTECT_SKILL_INDEX) != 0)
				continue;

			_user.push_back(party_member);
		}

		else
		{
			_user.push_back(party_member);
		}
	}

	if(_user.size() == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_CONDITION_ITEM_ALL_USER);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	idx = GetRandom(0, _user.size() - 1);
	_user[idx]->m_inventory.addItem(item);

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		ArtifactManager::instance()->addOwner(_user[idx], item, true);
	}

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_LOOT")
		<< LOG_VAL("account-id", pc->m_desc->m_idname ) << blank
		<< LOG_VAL("character-id", pc->m_desc->m_pChar->m_name ) << blank
		<< LOG_VAL("item-id", item->getDBIndex() ) << blank
		<< LOG_VAL("amount",item->Count() ) << blank
		//<< LOG_VAL("amount", money ) << blank
		<<endGer;
#endif // GER_LOG

	// LOG
	itemPickGameLog(_user[idx], item);

	// 원래 아이템 처리
	CNetMsg::SP disappmsg(new CNetMsg);
	CNetMsg::SP takemsg(new CNetMsg);
	CNetMsg::SP getmsg(new CNetMsg);

	int cx, cz;

	cx = item->m_cellX;
	cz = item->m_cellZ;

	ItemTakeMsg(takemsg, pc, item);
	ItemDisappearMsg(disappmsg, deleteVIndex);
	ItemGetMsg(getmsg, _user[idx], item);

	// 우선권 제거
	item->m_preferenceIndex = -1;

	// 땅에서 제거
	pc->m_pArea->ItemFromCell(item);

	// 메시지 보내고
	pc->m_pArea->SendToCell(takemsg, pc, true);
	pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);
	pc->m_party->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);

	return true;
}

//파티, 원정대 Box 아이템 획득
bool GetItemRaidOpenBox(CPC* pc, CItem* item)
{
	// validation
	if (!pc || !item || !(pc->m_pArea))
		return false;

	time_t t_now;
	bool bGiveItem = false;		// 아이템 가져 갔다!
	int BoxitemIndex = 4709;	// 상자아이템 인덱스

	int deleteVIndex = item->getVIndex();

	if(item->getDBIndex() != BoxitemIndex)
		return false;

	// 획득 시간 체크
	t_now = time(NULL);
	int nBoxItemTime = 0;

	nBoxItemTime = pc->GetRaidBoxItemTime();
	if(nBoxItemTime == 0)
	{
		// 박스아이템 획득 시간 설정
		pc->SetRaidBoxItemTime(t_now);
	}
	else if(nBoxItemTime > 0 && pc->m_isNotCoolBoxItem == false)
	{
		// 5분 체크
		if( (t_now - pc->GetRaidBoxItemTime()) <= (5 * 60))
		{
			// 더 이상 상자 주울 수 없음(획득 시간 5분)
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_5MIN_GETBOX);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
	}

	//Box 아이템이 이미 있는지 체크 (박스 아이템 인덱스 조정 필요)
	if(item->getDBIndex() == BoxitemIndex && pc->m_isNotCoolBoxItem == false)
	{
		if (pc->m_inventory.FindByDBIndex(item->getDBIndex()))
		{
			// 레이드 박스 아이템 이미 소유하고 있음
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_ALREADY_GETBOX);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
	}

	// 인벤에 넣기
	if (pc->m_inventory.addItem(item))
	{
		bGiveItem = true;

		// 박스아이템 획득 시간 설정
		pc->SetRaidBoxItemTime(t_now);

#ifdef GER_LOG
		GAMELOGGEM << init( 0 , "CHAR_LOOT")
				   << LOG_VAL("account-id", pc->m_desc->m_idname ) << blank
				   << LOG_VAL("character-id", pc->m_desc->m_pChar->m_name ) << blank
				   << LOG_VAL("item-id", item->getDBIndex() ) << blank
				   << LOG_VAL("amount",item->Count() ) << blank
				   <<endGer;
#endif // GER_LOG

		// LOG
		itemPickGameLog(pc, item);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	// 원래 아이템 처리
	CNetMsg::SP disappmsg(new CNetMsg);
	CNetMsg::SP takemsg(new CNetMsg);
	CNetMsg::SP getmsg(new CNetMsg);

	int cx, cz;

	cx = item->m_cellX;
	cz = item->m_cellZ;

	ItemTakeMsg(takemsg, pc, item);
	ItemDisappearMsg(disappmsg, deleteVIndex);
	ItemGetMsg(getmsg, pc, item);

	// 우선권 제거
	item->m_preferenceIndex = -1;

	// 땅에서 제거
	pc->m_pArea->ItemFromCell(item);

	// 받았다고 셋팅
	pc->m_bGiveItem = true;

	// 메시지 보내고
	pc->m_pArea->SendToCell(takemsg, pc, true);
	pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);

	if(pc->IsParty())
	{
		pc->m_party->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);
	}
	else if(pc->IsExped())
	{
		pc->m_Exped->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);
	}

	return true;
}

// 균등 파티 시 돈 갈라먹기(원정대)
void DivisionExpedMoney(CPC* pc, LONGLONG count)
{
	// validation
	if (!pc || count <= 0)
		return;

	int i,j;
	CExpedition* Exped = pc->m_Exped;
	int divCount = 0;				// 돈 갈라먹을 파티 멤버 수

	for (i=0; i < MAX_EXPED_GROUP; i++)
	{
		for (j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			CPC* pMemberPC = Exped->GetNearMember(pc, i,j);
			if (!pMemberPC)
				continue;

			divCount++;
		}
	}

	if (divCount < 1)
		return;

	// 돈 갈라내기
	//bool bTake = false;		// 돈을 받은 사람이 있나?: 없으면 아이템은 바닥에 그대로
	count = count / divCount;

	if (count < 1)
		return;

	GoldType_t Artimoney = 0;

	// 파티원에게 분배
	for (i = MAX_EXPED_GROUP - 1; i >= 0; i--)
	{
		for (j = MAX_EXPED_GMEMBER - 1; j >= 0; j--)
		{
			Artimoney = 0;

			CPC* pMemberPC = Exped->GetNearMember(pc, i,j);
			if (!pMemberPC)
				continue;

			// 갈른 돈 만들기
			int bonus = 0;

			if(pMemberPC->m_avPassiveAddition.money_nas > 0)
			{
				bonus += pMemberPC->m_avPassiveAddition.money_nas;	
			}
			if(pMemberPC->m_avPassiveRate.money_nas > 0)
			{
				bonus = count * (pMemberPC->m_avPassiveRate.money_nas - 100) / SKILL_RATE_UNIT;
			}

			Artimoney = count * pMemberPC->m_artiGold / 100;

			pMemberPC->m_inventory.increaseMoney(count + Artimoney, bonus);
		}
	}
}

// 균등원정대 아이템 랜덤으로 가져가지(원정대)
bool GetItemRandomExped(CPC* pc, CItem* item)
{
	// validation
	if (!pc || !item || !(pc->m_Exped) || !(pc->m_pArea))
		return false;

	int i,j;
	CExpedition* Exped = pc->m_Exped;
	bool bPreference = false;		// 이 원정대가 자격이 있는가
	int ExpedCount = 0;				// 받을 수 있는 자격이 있는 원정대원 수

	for (i=0; i < MAX_EXPED_GROUP; i++)
	{
		for (j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			CPC* pMemberPC = Exped->GetNearMember(pc, i,j);
			if (!pMemberPC)
				continue;

			if (item->m_preferenceIndex == pMemberPC->m_index)
				bPreference = true;

			ExpedCount++;
		}
	}

	// 우선권이 있는데 원정대가 자격없으면 return;
	if (item->m_preferenceIndex != -1 && !bPreference)
	{
		// 우선권이 없습니다 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	// 받을 사람이 없다.
	if (ExpedCount < 1)
		return false;

	bool bGiveItem = false;		// 아이템 가져 갔다!
	int idx,idx2;				// 결정된 파티원 인덱스

	int bitfield = 0;			// 결정됬으나 실패한 원정대원 인덱스 저장
	bool bAll = false;

	int deleteVIndex = item->getVIndex();

	//TODO
	/*
	1. 인벤토리가 빈 유저를 먼저 찾아 데이터를 저장해 놓고
	2. 랜덤으로 아이템을 넣어준다.
	*/

	std::vector<CPC*> _user;

	for(int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j = 0; j < pc->m_Exped->GetGroupMemberCount(i); j++)
		{
			CPC* exped_member = Exped->GetNearMember(pc, i, j);

			if(exped_member == NULL)
				continue;
			
			else if(exped_member->m_inventory.getEmptyCount() == 0)
				continue;

			else if(gserver->isActiveEvent(A_EVENT_EGGS_HUNT) &&
				item->getDBIndex() == 2148 &&
				exped_member->m_inventory.FindByDBIndex(item->getDBIndex()) != NULL)
				continue;

			else if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
			{
				if(exped_member->m_level <= PKMODE_LIMIT_LEVEL)
					continue;
				if(exped_member->m_assist.FindBySkillIndex(PVP_PROTECT_SKILL_INDEX) != 0)
					continue;

				_user.push_back(exped_member);
			}
			
			else
				_user.push_back(exped_member);
		}
	}

	if(_user.size() == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_CONDITION_ITEM_ALL_USER);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	idx = GetRandom(0, _user.size() - 1);
	_user[idx]->m_inventory.addItem(item);

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		ArtifactManager::instance()->addOwner(_user[idx], item, true);
	}

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_LOOT")
		<< LOG_VAL("account-id", pc->m_desc->m_idname ) << blank
		<< LOG_VAL("character-id", pc->m_desc->m_pChar->m_name ) << blank
		<< LOG_VAL("item-id", item->getDBIndex() ) << blank
		<< LOG_VAL("amount",item->Count() ) << blank
		//<< LOG_VAL("amount", money ) << blank
		<<endGer;
#endif // GER_LOG
	itemPickGameLog(_user[idx], item);
		
	// 원래 아이템 처리
	CNetMsg::SP disappmsg(new CNetMsg);
	CNetMsg::SP takemsg(new CNetMsg);
	CNetMsg::SP getmsg(new CNetMsg);

	int cx, cz;

	cx = item->m_cellX;
	cz = item->m_cellZ;

	ItemTakeMsg(takemsg, pc, item);
	ItemDisappearMsg(disappmsg, deleteVIndex);
	ItemGetMsg(getmsg, _user[idx], item);

	// 우선권 제거
	item->m_preferenceIndex = -1;

	// 땅에서 제거
	pc->m_pArea->ItemFromCell(item);

	// 메시지 보내고
	pc->m_pArea->SendToCell(takemsg, pc, true);
	pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);
	pc->m_Exped->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);

	return true;
}

bool GetItemGiveToBoss(CPC* pc, CItem* item)
{
	if(!pc || !item || !(pc->m_Exped) || !(pc->m_pArea))
		return false;

	int boss_idx = pc->m_Exped->GetBossIndex();
	CPC *pBoss = PCManager::instance()->getPlayerByCharIndex(boss_idx);
	if( !pBoss )
		return false;

	int deleteVIndex = item->getVIndex();

	if (pBoss->m_inventory.addItem(item))
	{
		GAMELOG << init("ITEM_PICK_TRIGGERITEM", pBoss)
				<< itemlog(item)
				<< end;
	}
	else
	{
		return false;
	}

	// 원래 아이템 처리
	CNetMsg::SP disappmsg(new CNetMsg);
	CNetMsg::SP takemsg(new CNetMsg);
	CNetMsg::SP getmsg(new CNetMsg);

	int cx, cz;

	cx = item->m_cellX;
	cz = item->m_cellZ;

	ItemTakeMsg(takemsg, pc, item);
	ItemDisappearMsg(disappmsg, deleteVIndex);
	ItemGetMsg(getmsg, pBoss, item);

	item->m_preferenceIndex = -1;
	pc->m_pArea->ItemFromCell(item);

	pc->m_pArea->SendToCell(takemsg, pc, true);
	pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);
	pc->m_Exped->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);

	return true;
}

// tch 의 어택리스트에 ch 정보 추가
// ch가 tch를 공격 ;;;;
CAttackChar* AddAttackList(CCharacter* ch, CCharacter* tch, int hate)
{
	if (ch == tch)
		return NULL;

	bool bCh = true;
	bool bTCh = true;

	CAttackChar* chAttackList = ch->m_attackList;
	CAttackChar* tchAttackList = tch->m_attackList;
	CAttackChar* ret = NULL;

	// 이미 어택리스에 있지만 다시 어택리스크가 된 경우
	// TargetHate 가 0인 넘이 있다면 도망갔다가 다시 온 경우
	// TargetHate를 초기값으로 셋팅
	while (chAttackList)
	{
		if (chAttackList->ch == tch)
		{
			bCh = false;
			chAttackList->m_targetHate += hate;
			break;
		}
		chAttackList = chAttackList->m_next;
	}

	while (tchAttackList)
	{
		if (tchAttackList->ch == ch)
		{
			bTCh = false;
			tchAttackList->m_targetHate += hate;
			ret = tchAttackList;
			if(IS_NPC(tch))
				tchAttackList->m_targetPulse = gserver->m_pulse;
			break;
		}
		tchAttackList = tchAttackList->m_next;
	}

	CAttackChar* attackCh = NULL;
	CAttackChar* attackTCh = NULL;

	if (bCh)
	{
		attackCh = new CAttackChar;
		attackCh->ch = tch;
		ADD_TO_BILIST(attackCh, ch->m_attackList, m_prev, m_next);
	}

	if (bTCh)
	{
		attackTCh = new CAttackChar;
		attackTCh->ch = ch;
		attackTCh->m_bFirstAttack = true;
		ADD_TO_BILIST(attackTCh, tch->m_attackList, m_prev, m_next);
		ret = attackTCh;
	}

	if (bCh && bTCh)
	{
		if (IS_NPC(tch))
			attackTCh->m_targetPulse = gserver->m_pulse;
		attackTCh->m_targetHate += hate;
	}

	return ret;
}

// ch 어택리스트에서 제거
void DelAttackList(CCharacter* ch)
{
	if (ch == NULL
			|| ch->m_attackList == NULL)
	{
		return;
	}

	CAttackChar* attCh;
	CAttackChar* attChNext = ch->m_attackList;
	CAttackChar* attTCh;
	CAttackChar* attTChNext;

	while ((attCh = attChNext))
	{
		attChNext = attCh->m_next;

		attTChNext = attCh->ch->m_attackList;
		while ((attTCh = attTChNext))
		{
			attTChNext = attTCh->m_next;
			if (attTCh->ch == ch)
			{
				REMOVE_FROM_BILIST(attTCh, attCh->ch->m_attackList, m_prev, m_next);
				delete attTCh;
				attTCh = NULL;
				break;
			}
		}

		REMOVE_FROM_BILIST(attCh, ch->m_attackList, m_prev, m_next);
		delete attCh;
		attCh = NULL;
	}
}

// ch 어택리스트에서 tch만 제거
void DelTargetFromAttackList(CCharacter* ch, CCharacter* tch)
{
	CAttackChar* attCh;
	CAttackChar* attChNext = ch->m_attackList;
	CAttackChar* attTCh;
	CAttackChar* attTChNext;

	while ((attCh = attChNext))
	{
		attChNext = attCh->m_next;

		if (attCh->ch == tch)
		{
			attTChNext = attCh->ch->m_attackList;
			while ((attTCh = attTChNext))
			{
				attTChNext = attTCh->m_next;
				if (attTCh->ch == ch)
				{
					REMOVE_FROM_BILIST(attTCh, attCh->ch->m_attackList, m_prev, m_next);
					delete attTCh;
					attTCh = NULL;
					break;
				}
			}

			REMOVE_FROM_BILIST(attCh, ch->m_attackList, m_prev, m_next);
			delete attCh;
			attCh = NULL;
			break;
		}
	}
}

// 정당방위 리스트에 추가
void AddRaList(CPC* of, CPC* df)
{
	// 피해자 정방 리스트에 가해자 추가
	CRaChar* raOf = new CRaChar;
	raOf->m_raTarget = of;
	raOf->m_raPulse = RAMODE_USE_PULSE;
	raOf->m_bAttacker = true;
	ADD_TO_BILIST(raOf, df->m_raList, m_prev, m_next);
	df->SetPlayerState(PLAYER_STATE_RAMODE);

	// 가해자 정방 리스트에 피해자 추가
	CRaChar* raDf = new CRaChar;
	raDf->m_raTarget = df;
	raDf->m_bAttacker = false;
	ADD_TO_BILIST(raDf, of->m_raList, m_prev, m_next);

	CNetMsg::SP rmsg(new CNetMsg);
	RightAttackMsg(rmsg, of, MSG_RIGHT_ATTACK_ADD);
	SEND_Q(rmsg, of->m_desc);
	SEND_Q(rmsg, df->m_desc);
}

// 정당방위 리스트에서 제거
void DelRaList(CPC* pc, bool bForce)
{
	CRaChar* raPC;
	CRaChar* raPCNext = pc->m_raList;

	CRaChar* raTPC;
	CRaChar* raTPCNext;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RightAttackMsg(rmsg, pc, MSG_RIGHT_ATTACK_DEL);

		while ((raPC = raPCNext))
		{
			raPCNext = raPC->m_next;
			raTPCNext = raPC->m_raTarget->m_raList;

			while ((raTPC = raTPCNext))
			{
				raTPCNext = raTPC->m_next;

				if (raTPC->m_raTarget == pc)
				{
					if (!raTPC->m_bAttacker && !bForce)
						return;

					SEND_Q(rmsg, raTPC->m_raTarget->m_desc);
					REMOVE_FROM_BILIST(raTPC, raPC->m_raTarget->m_raList, m_prev, m_next);
					delete raTPC;
					raTPC = NULL;
					break;
				}
			}

			SEND_Q(rmsg, raPC->m_raTarget->m_desc);

			REMOVE_FROM_BILIST(raPC, pc->m_raList, m_prev, m_next);
			delete raPC;
			raPC = NULL;
		}
	}
}

// pc 정당방위 리스트에 tpc가 있는가
bool IsRaList(CPC* pc, CPC* tpc)
{
	CPC* findPC = PCManager::instance()->getPlayerByCharIndex(pc->m_index);
	if (findPC == NULL)
	{
		GAMELOG << init("PCMEM_CHECK_ERROR", pc) << "IsRaList" << end;
		return false;
	}

	CRaChar* raList = pc->m_raList;

	while (raList)
	{
		if (raList->m_raTarget == tpc)
			return true;

		raList = raList->m_next;
	}

	return false;
}

bool IsFirstAttackInAttackList(CPC* of, CPC* df)
{
	CAttackChar* pAttackChar = df->m_attackList;
	while (pAttackChar)
	{
		if (pAttackChar->ch == of && pAttackChar->m_bFirstAttack)
			return true;
		pAttackChar = pAttackChar->m_next;
	}
	return false;
}

// 공격 당하는 npc의 동족찾기 : 있으면 같이 공격
void FindFamilyInNear(CCharacter* of, CNPC* npc)
{
	// Cell 2개 정도에서
	int sx = npc->m_cellX - 2;
	int ex = npc->m_cellX + 2;
	int sz = npc->m_cellZ - 2;
	int ez = npc->m_cellZ + 2;

	int x, z;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= npc->m_pArea->m_size[0])
			continue;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= npc->m_pArea->m_size[1])
				continue;

			CCharacter* ch = npc->m_pArea->m_cell[x][z].m_listChar;
			while (ch)
			{
				if (IS_NPC(ch) && TO_NPC(ch) != npc)
				{
					CNPC* tnpc = TO_NPC(ch);

					// 동족이고 시야 2배 이내 거리이고 disableTime이 셋팅 되어 있지 않을때
					// attacklist가 비어 있을 때
					if (npc->m_proto->m_family != -1 &&
							tnpc->m_proto->m_family == npc->m_proto->m_family &&
							GetDistance(npc, tnpc) < npc->m_proto->m_sight * 2 &&
							tnpc->m_disableTime == 0 && !tnpc->IsBlind()
							&& tnpc->m_attackList == NULL)
						AddAttackList(of, tnpc, HATE_FIRST_ATTACK);
				}
				ch = ch->m_pCellNext;
			}
		}
	}
}

////////////////////
// Function name	: GetSkillPrototypes
// Description	    : proto, level, magicindex로 각각의 프로토타입을 얻는다
// Return type		: bool
//                  : 정상일때 true
bool GetSkillPrototypes(const CSkillProto* proto, int level, int magicindex, const CSkillLevelProto** levelproto, const CMagicProto** magic, const CMagicLevelProto** magiclevel)
{
	*levelproto = NULL;
	*magic = NULL;
	*magiclevel = NULL;

	if (proto == NULL)
		return false;

	if (magicindex < 0 || magicindex >= MAX_SKILL_MAGIC)
		return false;

	*levelproto = proto->Level(level);
	if (*levelproto == NULL)
		return false;

	*magic = (*levelproto)->m_magic[magicindex];
	if (*magic == NULL)
		return false;

	*magiclevel = (*magic)->Level((*levelproto)->m_magicLevel[magicindex]);
	if (*magiclevel == NULL)
		return false;

	return true;
}

// of -> df에게 hate만큼 HATE 수치 증가
CAttackChar* ApplyHate(CCharacter* of, CCharacter* df, int hate, bool bApplyFamily)
{
	// 선공 검사
	bool bFirstAttack = false;

	// df의 리스트가 비었을 때에만 선공
	if (df->m_attackList == NULL)
	{
		bFirstAttack = true;
		hate += HATE_FIRST_ATTACK;
	}

	// 리스트에 추가
	CAttackChar* ret = AddAttackList(of, df, hate);

	// 동족에게 적용 : df == NPC일 경우
	if (bApplyFamily && IS_NPC(df))
		FindFamilyInNear(of, TO_NPC(df));

	return ret;
}

////////////////////
// Function name	: ApplyHateByDamage
// Description	    : of가 df를 damage만큼 공격시 해당하는 hate 수치를 적용
void ApplyHateByDamage(CCharacter* of, CCharacter* df, char hittype, int damage)
{
	if (hittype != HITTYPE_MISS)
	{
		int hate = 0;

		// of가 NPC일 경우 hate 증가 안함
		// damage에 따른 hate 수치 계산
		if (!IS_NPC(of))
		{
			hate += 10000 * damage / df->m_maxHP;
			if (hate < 1)
				hate = 1;
		}

		CAttackChar* p = ApplyHate(of, df, hate, true);
		if (p)
		{
			if (p->m_damage + damage > 1)
				p->m_damage += damage;
		}
	}
}

////////////////////
// Function name	: ApplyDamage
// Description	    : of가 df를 damageType형태의 proto를 가지고 flag형태로 damage만큼 공격
//                  : 해당하는 damage메시지와 proto에 다른 effect메시지를 전달
void ApplyDamage(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, const CSkillProto* proto, const CMagicProto* magic, int damage, char flag, bool isCharge)
{
	int realdamage = 0;

	// PvP에서는 데미지 25%만
	bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of) || IS_APET(of) );
	bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df) || IS_APET(df) );
	if (bIsPCof && bIsPCdf)
		damage /= 4;

	if (flag != HITTYPE_MISS)
	{
		if (damage < 1)
			damage = 1;

		if (IS_PET(df))
		{
			damage = 1;
			// 050309: bw 배고픔 20프로 미만일때 데미지 2씩 감소
			CPet* p = TO_PET(df);
			if( p )
			{
				if( p->GetHungryPoint() < 20 )
					damage = 2;
			}
		}
	}
	else
		damage = 0;

	if (damage > 0)
	{
		// 절망 처리
		if (df->m_assist.m_state & AST_DESPAIR)
		{
			damage += df->m_assist.m_avAddition.despair
					  + damage * df->m_assist.m_avRate.despair / SKILL_RATE_UNIT;
			df->m_assist.CancelDespair();
		}

		if (df->m_linkSource && df != df->m_linkSource && CheckInNearCellExt(df, df->m_linkSource))
		{
			int linkdamage = damage / 2;
			damage -= linkdamage;
			if (linkdamage > 0)
				ProcAttack(of, df->m_linkSource, MSG_DAMAGE_LINK, NULL, 0, linkdamage);
		}

		if (IS_ELEMENTAL(df))
		{
			CElemental* pElemental = TO_ELEMENTAL(df);
			CPC* pPC = pElemental->GetOwner();
			if (pPC && (pPC->m_assist.m_state & AST_MANASCREEN) && CheckInNearCellExt(pElemental, pPC))
			{
				damage -= pPC->m_assist.m_avAddition.manascreen
						  + damage * pPC->m_assist.m_avRate.manascreen / SKILL_RATE_UNIT;
				pPC->m_mp -= pPC->m_maxMP * 3 / 100;
				if (pPC->m_mp <= 0)
				{
					pPC->m_mp = 0;
					pPC->m_assist.CancelManaScreen();
				}
				pPC->m_bChangeStatus = true;
			}
		}

		if (damage < 1)
			damage = 1;

#ifdef ASSIST_REDUCE_SKILL
		if (proto)
		{
			if (GetRandom(1, 10000) <=
					df->m_avPassiveAddition.reduceSkillProb)
			{
				damage = damage - df->m_avPassiveAddition.reduceSkill -
						 damage * df->m_avPassiveRate.reduceSkill /
						 SKILL_RATE_UNIT;
			}

			if (GetRandom(1, 10000) <=
					df->m_assist.m_avAddition.reduceSkillProb)
			{
				damage = damage - df->m_assist.m_avAddition.reduceSkill -
						 damage * df->m_assist.m_avRate.reduceSkill /
						 SKILL_RATE_UNIT;

				df->m_assist.DecreaseCount(MT_REDUCE, MST_REDUCE_SKILL);
			}

			if (damage < 0) /* reduce skill can cause damage zero */
				damage = 0;
		}
#endif
		damage = CalcNewDamage(of, df, damage);

		if (damage < 0)
			damage = 0;

		// HP 감소
		if (damage > df->m_hp)
			realdamage = df->m_hp;
		else
			realdamage = damage;

		bool bSendOFStatusMsg = false;

		// HP/MP 스틸
		if (IS_PC(of) && realdamage > 0)
		{
			CPC* pPCOffense = TO_PC(of);

//#ifdef HP_STEEL_POTION
			if (pPCOffense->m_assist.m_avAddition.hcHPSteelPotion == true )
			{
				int prob = GetRandom( 1, 100 );
				if( prob > 70 ) // 30%
				{
					int nStealHP = realdamage * 5 / 100;
					if( nStealHP > 0 )
					{
						pPCOffense->m_hp += nStealHP;
						if( pPCOffense->m_hp > pPCOffense->m_maxHP )
							pPCOffense->m_hp = pPCOffense->m_maxHP;
						pPCOffense->m_bChangeStash = true;
						bSendOFStatusMsg = true;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							EffectEtcMsg(rmsg, pPCOffense, MSG_EFFECT_ETC_HP_STEEL);
							pPCOffense->m_pArea->SendToCell(rmsg, pPCOffense, true);
						}
					}
				}
			}
			else
//#endif // HP_STEEL_POTION
				if (pPCOffense->m_opHPSteal > 0)
				{
					if (GetRandom(1, 100) > 95) // 5%
					{
						int nStealHP = realdamage * pPCOffense->m_opHPSteal / 100;
						if (nStealHP > 0)
						{
							pPCOffense->m_hp += nStealHP;
							if (pPCOffense->m_hp > pPCOffense->m_maxHP)
								pPCOffense->m_hp = pPCOffense->m_maxHP;
							pPCOffense->m_bChangeStatus = true;
							bSendOFStatusMsg = true;
						}
					}
				}

//#ifdef MP_STEEL_POTION
			if (pPCOffense->m_assist.m_avAddition.hcMPSteelPotion == true )
			{
				int prob = GetRandom( 1, 100 );
				if( prob > 70 ) // 30%
				{
					int nStealMP = pPCOffense->m_maxMP * 10 / 100;
					if (nStealMP > 0)
					{
						pPCOffense->m_mp += nStealMP;
						if (pPCOffense->m_mp > pPCOffense->m_maxMP)
							pPCOffense->m_mp = pPCOffense->m_maxMP;
						pPCOffense->m_bChangeStatus = true;
						bSendOFStatusMsg = true;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							EffectEtcMsg(rmsg, pPCOffense, MSG_EFFECT_ETC_MP_STEEL);
							pPCOffense->m_pArea->SendToCell(rmsg, pPCOffense, true);
						}
					}
				}
			}
			else
//#endif // MP_STEEL_POTION
				if (pPCOffense->m_opMPSteal > 0)
				{
					if (GetRandom(1, 100) > 95) // 5%
					{
						int nStealMP = realdamage * pPCOffense->m_opMPSteal / 100;
						if (nStealMP > 0)
						{
							pPCOffense->m_mp += nStealMP;
							if (pPCOffense->m_mp > pPCOffense->m_maxMP)
								pPCOffense->m_mp = pPCOffense->m_maxMP;
							pPCOffense->m_bChangeStatus = true;
							bSendOFStatusMsg = true;
						}
					}
				}
		}

		if (magic
#ifdef ASSIST_REDUCE_SKILL
				&& realdamage > 0
#endif
		   )
		{
			switch (magic->m_type)
			{
			case MT_ATTACK:
				switch (magic->m_subtype)
				{
				case MST_ATTACK_DRAIN:
					if (!IS_PET(of) && !IS_PET(df))
					{
						if (IS_PC(of) && IS_PC(df))
							of->m_hp += realdamage / 2;
						else
							of->m_hp += realdamage;
						if (of->m_hp > of->m_maxHP)
							of->m_hp = of->m_maxHP;

						if (IS_PC(of))
						{
							CPC* opc = TO_PC(of);
							opc->m_bChangeStatus = true;
						}
						bSendOFStatusMsg = true;
					}
					break;
				case MST_ATTACK_ONESHOTKILL:
					// 15% 미만이면 즉사가 적용됨
					if ( IS_NPC(df) && df->m_hp <= df->m_maxHP * 15 / 100 )
					{
						damage = df->m_hp;
					}
					break;
				}
				break;
			}
		}

		if ( IS_APET(df) )
		{
			CAPet* apet = TO_APET(df);
			if( apet )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExAPetStatusMsg( rmsg, apet);
				SEND_Q( rmsg, apet->GetOwner()->m_desc );
			}
		}

		if (bSendOFStatusMsg)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			CharStatusMsg(rmsg, of, 0);
			df->m_pArea->SendToCell(rmsg, of, true);
		}

		// npc가 리플렉션 데미지를 받고 죽으면 npc=null 되어 서버 다운 버그 수정
		switch( damageType )
		{
		case MSG_DAMAGE_REFLEX:
		case MSG_DAMAGE_LINK:
			if( IS_NPC(df) )
			{
				if( damage >= df->m_hp )
					damage = df->m_hp -1;
			}
			break;
		default:
			break;
		}

#ifdef TLD_EVENT_SONG
		if (IS_NPC(df))
		{
			CNPC* pNpc = TO_NPC(df);
			if (pNpc->m_proto->m_index == 1622 || pNpc->m_proto->m_index == 1623)
			{
				damage = 1;
			}
		}
#endif

		df->m_hp -= damage;

		if ( of->m_assist.m_state & AST_INVERSE_DAMAGE && damageType != MSG_DAMAGE_COMBO )
		{
			// 역 데미지 디버프에 걸린 몬스터이면 몬스터 콤보 이펙트 데미지를 제외한 모든 데미지를 회복으로 바꾼다.
			df->m_hp += damage * 2;
		}

		if(IS_APET(df))
		{
			if(df->m_hp >= df->m_maxHP + TO_APET(df)->GetOwner()->m_opJewelElementPetHPUP)
			{
				df->m_hp = df->m_maxHP + TO_APET(df)->GetOwner()->m_opJewelElementPetHPUP;
			}
		}
		else if (df->m_hp >= df->m_maxHP)
			df->m_hp = df->m_maxHP;
		if (df->m_hp <= 0)
			df->m_hp = 0;
	}
	//아이템에 옵션(패시브, 공격형, 방어형) 이 있는 경우 적용.
	//TODO	1단계 : 캐릭터가 입고 있는 아이템을 검사한다.
	//		2단계 : 아이템에 있는 옵션중에 패시브이면 항상 적용, 공격형이면 공격시 적용, 방어형이면 피해를 입었을 때 적용시킨다.
	//		3단계 : 스킬 메시지를 보낸다.

	//성수 아이템 데미지 최종 데미지에서 +@로 들어감.

	int holyItemIndex = -1;

	if(IS_PC(of) && flag != HITTYPE_MISS )
	{
		CPC* pc = TO_PC(of);
		if(pc->holy_water_item != NULL && isCharge == false)
		{
			holyItemIndex = pc->holy_water_item->getDBIndex();
			damage = HolyWaterData::instance()->getDamage(pc, df, magic, damage);			
		}
	}

	// 결과 메시지
	if (proto)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		DamageMsg(rmsg, of, df, damageType, proto->m_index, damage, flag, holyItemIndex);
#if defined (LC_GAMIGO) || defined (LC_USA) || defined (LC_RUS)					//보스몬스터 로그 게더링
		if(IS_NPC(df)) 															//보스NPC에게 공격하는 경우 로그
		{
			CNPC *npc = TO_NPC(df);
			if(npc->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				GAMELOG << init("BOSS DAMAGE IN") << df->m_index << df->m_name << delim << damage << delim << proto->m_index << delim << of->m_name << delim;
				CAssistData *p = NULL;
				CAssistData *pNext = NULL;

				p = of->m_assist.m_help.getHead();
				while((pNext = p))
				{
					p = p->m_next;
					GAMELOG << pNext->m_proto->m_index << "(" << pNext->m_level << ") ";
				}
				GAMELOG << end;
			}
		}
		if(IS_NPC(of)) 															//보스NPC에게 공격당할 경우 로그
		{
			CNPC *npc = TO_NPC(of);
			if(npc->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				GAMELOG << init("BOSS DAMAGE OUT") << of->m_index << of->m_name << delim << damage << delim << proto->m_index << delim << df->m_name << delim;
				CAssistData *p = NULL;
				CAssistData *pNext = NULL;

				p = df->m_assist.m_help.getHead();
				while((pNext = p))
				{
					p = p->m_next;
					GAMELOG << pNext->m_proto->m_index << "(" << pNext->m_level << ") ";
				}
				GAMELOG << end;
			}
		}
#endif
		df->m_pArea->SendToCell(rmsg, df, true);
	}
	else
	{
		//일반 공격일때
		CNetMsg::SP rmsg(new CNetMsg);
		DamageMsg(rmsg, of, df, damageType, -1, damage, flag, holyItemIndex);
#if defined (LC_GAMIGO) || defined (LC_USA) || defined (LC_RUS)					//보스몬스터 로그 게더링
		if(IS_NPC(df)) 															//보스NPC에게 공격할 경우 로그
		{
			CNPC *npc = TO_NPC(df);
			if(npc->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				GAMELOG << init("BOSS DAMAGE IN") << df->m_index << df->m_name << delim << damage << delim << -1 << delim << of->m_name << delim;
				CAssistData *p = NULL;
				CAssistData *pNext = NULL;

				p = of->m_assist.m_help.getHead();
				while((pNext = p))
				{
					p = p->m_next;
					GAMELOG << pNext->m_proto->m_index << "(" << pNext->m_level << ") ";
				}
				GAMELOG << end;
			}
		}
		if(IS_NPC(of)) 															//보스NPC에게 공격당할 경우 로그
		{
			CNPC *npc = TO_NPC(of);
			if(npc->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				GAMELOG << init("BOSS DAMAGE OUT") << of->m_index << of->m_name << delim << damage << delim << -1 << delim << df->m_name << delim;
				CAssistData *p = NULL;
				CAssistData *pNext = NULL;

				p = df->m_assist.m_help.getHead();
				while((pNext = p))
				{
					p = p->m_next;
					GAMELOG << pNext->m_proto->m_index << "(" << pNext->m_level << ") ";
				}
				GAMELOG << end;
			}
		}
#endif
		df->m_pArea->SendToCell(rmsg, df, true);
	}

	//무적 버프가 있는 경우에(공격가능모드)는 해당 버프를 지워준다.
	if (of->m_assist.FindBySkillIndex(IMMOTAL_BUF) && of->m_bFirstDoIt == true)
	{
		of->m_assist.CureBySkillIndex(IMMOTAL_BUF);
	}
}

////////////////////
// Function name	: CalcPKPoint
// Description	    : of에 의해 df가 사망시 of, df의 성향 수치 계산
//                  : bDeadPet가 true이면 Pet 사망에 의한 처리
void CalcPKPoint(CPC* of, CPC* df, bool bDeadPet)
{
	//공격자가 무소속이 아니라면 return;
	if (of->m_pZone->isRVRZone() && of->getSyndicateType() != 0)
		return;
	
	// 이전에 카오인지 검사

#ifdef FREE_PK_SYSTEM
	if( !gserver->m_bFreePk )
	{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
		if( !gserver->m_bDisablePKPaenalty )
		{
#endif // MAL_DISABLE_PKPENALTY

			bool isPKChar = (of->m_pkPenalty < -9) ? true : false;

			if (!df->IsChaotic() || bDeadPet)
			{
				// 멀쩡한 사람 죽임 또는 펫 죽임
#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 멀쩡한 사람을 죽였으니 카오성향으로 증가
				if( of->IsChaotic() ) // 카오가 일반이나 헌터를 죽였을 경우
				{
					int penalty = 0;
					if( of->m_pkPenalty >= PK_CHAOTIC_POINT_1 )
						penalty = -8000;
					else if ( of->m_pkPenalty >= PK_CHAOTIC_POINT_2 )
						penalty = -10000;
					else
						penalty = -15000;

					of->AddPkPenalty( penalty );
				}
				else // 일반이나 헌터가 일반 혹은 헌터를 죽였을 경우 무조건 카오로 만든다.
					of->m_pkPenalty = PK_CHAOTIC_POINT_1;
#else
				of->m_pkPenalty -= 10;
				if (!df->IsSetPlayerState(PLAYER_STATE_PKMODE))
					of->m_pkPenalty -= 10;
				if (of->m_pkPenalty < -155)
					of->m_pkPenalty = -155;
#endif // REFORM_PK_PENALTY_201108 // PK 패널티 리폼
				of->m_pkCount -= 1;
				if (of->m_pkCount < -110)
					of->m_pkCount = -110;
			}
			else
			{
				// 나쁜넘 죽임
#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 카오를 죽였다면
				if( of->m_pkPenalty >= 0 && of->m_pkPenalty < PK_HUNTER_POINT_1 ) // 특정 성향 범위에서만 카오를 죽일때 성향 수치를 준다.
				{
					int nlevel = df->m_level - of->m_level;
					int pkPenalty = 0;
					if( nlevel > 4 )
						pkPenalty += 3000;
					else if( nlevel > -5 )
						pkPenalty += 1000;
					else if( nlevel <= -5 )
						pkPenalty += 200;

					if( of->m_assist.m_avRate.pkDispositionPointValue > 0 )
					{
						pkPenalty = pkPenalty * of->m_assist.m_avRate.pkDispositionPointValue;
						of->m_assist.CureByItemIndex(7474);	// 성향 수치 상승 증폭제
						of->m_assist.CureByItemIndex(7475);	// 성향 수치 상승 증폭제
						of->m_assist.CureByItemIndex(7476);	// 성향 수치 상승 증폭제
					}

					of->AddPkPenalty( pkPenalty );
				}

				if( df->m_level >= of->m_level )
				{
					of->m_pkCount += 1;
					if (of->m_pkCount > 110)
						of->m_pkCount = 110;
				}
#else // REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 카오를 죽였다면
				// 동렙 이상이면
				if (df->m_level >= of->m_level)
				{
					if( !gserver->m_bNonPK )
						of->m_pkPenalty += 5;

					if (of->m_pkPenalty > 155)
						of->m_pkPenalty = 155;

					of->m_pkCount += 1;

					if (of->m_pkCount > 110)
						of->m_pkCount = 110;
				}
#endif // REFORM_PK_PENALTY_201108  PK 패널티 리폼 :: 카오를 죽였다면
			}

#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼
			if (of->m_pkPenalty < 0 )
#else // REFORM_PK_PENALTY_201108 // PK 패널티 리폼
			if (of->m_pkPenalty < -9)
#endif // REFORM_PK_PENALTY_201108 // PK 패널티 리폼
			{
				if (!isPKChar || of->m_pkRecoverPulse <= 0)
					of->m_pkRecoverPulse = TIME_ONE_HOUR;
			}
#ifdef MAL_DISABLE_PKPENALTY
		}
#endif // MAL_DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
	}
#endif // FREE_PK_SYSTEM

	of->m_bChangeStatus = true;


	{
		// 페널티 수치 변경 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, of, 0);
		of->m_pArea->SendToCell(rmsg, of, false);
	}

	of->m_bChangeStatus = true;
}

void CalcWarPoint(CCharacter* of, CCharacter* df)
{
	// 공성이 이루어지고 있는 존에서
	int zoneindex = of->m_pZone->m_index;
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle == NULL)
		return ;

	// 공성중에 길드장 사망시 포인트 감소 : 10 %
	if (IS_PC(df))
	{
		CPC* dpc = TO_PC(df);
		if (dpc->m_guildInfo && dpc->m_guildInfo->guild())
		{
			if (dpc->GetJoinFlag(dpc->m_pZone->m_index) == WCJF_ATTACK_GUILD)
			{
				if (castle->GetState() == WCSF_WAR_CASTLE)
				{
					if (castle->IsTop3Guild(dpc->m_guildInfo->guild()->index()))
					{
						int point=0;
						if(dpc->m_guildInfo->pos() == MSG_GUILD_POSITION_BOSS)
							point = (int)(point / 10.0 + 0.5);
						else if(dpc->m_guildInfo->pos() == MSG_GUILD_POSITION_OFFICER)
							point = (int)(point / 20.0 + 0.5);

						if(point > 0)
						{
							castle->IncreaseTeamPoint(dpc->m_guildInfo->guild()->index(), -point);
							PCManager::instance()->sendGuildWarPointMsg(castle, dpc->m_pZone->m_index);
						}
					}
				}
			}
		}
	} // 공성중에 길드장 사망시 포인트 감소 : 10 %

	if (castle->GetState() != WCSF_WAR_FIELD)
		return ;

	int mode = 0;			// 포인트 계산 방법, 없음(0), 수성1포인트(1), 공성1포인트(2), 공성20포인트(3)

	// 죽인 사람이 PC인가?
	if (IS_PC(of))
	{
		CPC* opc = TO_PC(of);

		if (IS_DEFENSE_TEAM(opc->GetJoinFlag(zoneindex)))
		{
			// 수성측이 죽인 거라면, 죽은 사람이 공성 측인가?
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (IS_ATTACK_TEAM(dpc->GetJoinFlag(zoneindex)))
				{
					mode = 1;
				}
			}
		}
		else if (opc->GetJoinFlag(zoneindex) == WCJF_ATTACK_GUILD)
		{
			// 공성길드원이 죽인 거라면
			// PC인가? 수호병인가
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (dpc->GetJoinFlag(zoneindex) != WCJF_NONE)
				{
					mode = 2;
				}
			}
			else if (IS_NPC(df))
			{
				CNPC* dnpc = TO_NPC(df);
				if (dnpc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
				{
					mode = 3;
				}
			}
		}
	}
	else if (IS_NPC(of))
	{
		// 수호병이 죽였다면
		CNPC* onpc = TO_NPC(of);
		if (onpc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			// 죽은게 공성측
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (IS_ATTACK_TEAM(dpc->GetJoinFlag(zoneindex)))
				{
					mode = 1;
				}
			}
		}
	}
	else
		return ;

	int increasePoint=0;
	switch(mode)
	{
	case 1:
	case 2:
		if (IS_PC(df))
		{
			CPC* dpc = TO_PC(df);
			int joinflag = dpc->GetJoinFlag(zoneindex);

			if(joinflag == WCJF_OWNER							// 길드참여일 때
					|| joinflag == WCJF_DEFENSE_GUILD
					|| joinflag == WCJF_ATTACK_GUILD)
			{
				if(dpc->m_guildInfo)
				{
					MSG_GUILD_POSITION_TYPE postype = dpc->m_guildInfo->pos();
					if(postype == MSG_GUILD_POSITION_BOSS)			increasePoint=20;
					else if(postype == MSG_GUILD_POSITION_OFFICER)	increasePoint=7;
					else if(postype != MSG_GUILD_POSITION_UNKNOWN)	increasePoint=5;
				}
			}
			else if(joinflag == WCJF_DEFENSE_CHAR				// 용병참여일 때
					|| joinflag == WCJF_ATTACK_CHAR)
			{
				// 용병은 5점
				increasePoint=5;
			}
		}
		break;
	case 3:														// 수호병 NPC 일 때
		increasePoint=3;
		break;
	default:
		return ;
	}

	switch (mode)
	{
	case 1:			// 수성측 1포인트
		{
			int totalDamage = 0;
			int damage = 0;	// 수성측이 준 대미지 총합

			// PC를 공격했던 캐릭터 루프를 돌면서 수성측이 준 대미지 총합을 구한다
			CAttackChar* p = df->m_attackList;
			while (p)
			{
				if (p->ch)
				{
					totalDamage += p->m_damage;

					switch (p->ch->m_type)
					{
					case MSG_CHAR_PC:
						// 해당 존 수성측 유저라면
						if (IS_DEFENSE_TEAM(p->pc->GetJoinFlag(zoneindex)))
							damage += p->m_damage;
						break;
					case MSG_CHAR_NPC:
						// 수호병이라면
						if (p->npc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
							damage += p->m_damage;
						break;
					default:
						return ;
					}
				}	// if (p->ch)

				p = p->m_next;
			}	// while (p)

			// 체력의 절반 이상 대미지를 준경우 포인트 증가
			if (totalDamage / 2 <= damage && damage > 0)
			{
				castle->IncreaseTeamPoint(0, increasePoint);
			}
		}
		break;

	case 2:			// 공성측 1포인트
	case 3:			// 공성측 5포인트
		{
			int totalDamage = 0;
			int countAttackGuild = castle->GetCountAttackGuild();		// 공성 길드 수
			int* indexAttackGuild = new int[countAttackGuild];			// 공성 길드 인덱스
			int* damagePerAttackGuild = new int[countAttackGuild];		// 공성 길드별 대미지
			int maxDamageGuild = 0;										// 대미지를 가장 많이 준 길드 배열 인덱스
			int damageAttackTeam = 0;									// 공성측(길드, 용병)이 준 대미지 총합

			// 배열 초기화
			memset(indexAttackGuild, 0, sizeof(int) * countAttackGuild);
			memset(damagePerAttackGuild, 0, sizeof(int) * countAttackGuild);

			// df 공격했던 캐릭터 루프를 돌면서
			CAttackChar* p = df->m_attackList;
			while (p)
			{
				if (p->ch)
				{
					totalDamage += p->m_damage;

					if (IS_PC(p->ch))
					{
						// 해당 존 공성측 유저 중에, 길드 정보가 있는 유저이면
						if (p->pc->GetJoinFlag(zoneindex) == WCJF_ATTACK_GUILD && p->pc->m_guildInfo && p->pc->m_guildInfo->guild())
						{
							int i;
							for (i = 0; i < countAttackGuild; i++)
							{
								if (indexAttackGuild[i] == p->pc->m_guildInfo->guild()->index() || indexAttackGuild[i] == 0)
								{
									// 해당 길드 대미지 합계에 더하고
									indexAttackGuild[i] = p->pc->m_guildInfo->guild()->index();
									damagePerAttackGuild[i] += p->m_damage;

									// 공격측 길드 중에 가장 대미지를 많이 준 길드를 계산
									if (i != 0 && damagePerAttackGuild[i] >= damagePerAttackGuild[maxDamageGuild])
										maxDamageGuild = i;

									break;	// break for
								}
							}
						}
						if (IS_ATTACK_TEAM(p->pc->GetJoinFlag(zoneindex)))
							damageAttackTeam += p->m_damage;
					}
				}	// if (p->ch)

				p = p->m_next;
			}	// while (p)

			// 누적 대미지(또는 최대 체력)와 공격팀의 전체 대미지를 비교해서 공격팀이 50% 이상을 차지해야 포인트가 증가
			if (totalDamage / 2 <= damageAttackTeam && indexAttackGuild[maxDamageGuild] != 0)
			{
				castle->IncreaseTeamPoint(indexAttackGuild[maxDamageGuild], increasePoint);
			}

			delete [] indexAttackGuild;
			delete [] damagePerAttackGuild;
		}
		break;
	default:
		return ;
	}
	PCManager::instance()->sendGuildWarPointMsg(castle, zoneindex);
}

void DropWarCastleTokenDeadPC(CCharacter* df)
{
	if(!IS_PC(df))
		return ;

	CPC* dpc = TO_PC(df);

	int zoneindex=df->m_pZone->m_index;
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if(castle == NULL)
		return ;

	if(castle->GetState() == WCSF_NORMAL)
		return ;

	if( dpc->GetJoinFlag(zoneindex) == WCJF_NONE  )
		return ;

	if ( !castle->IsInInnerCastle(dpc) )
		return ;

	// 확률 계산
	if(GetRandom(1, 100) > 5)
		return ;

	int maxdamage = 0;
	CAttackChar* p = NULL;
	CAttackChar* pNext = dpc->m_attackList;

	CCharacter* preference = NULL;
	while( (p = pNext) )
	{
		pNext = pNext->m_next;
		if(!p->ch)
			continue;
		if(!IS_PC(p->ch))
			continue;

		if ((p->ch->m_pZone->m_index != df->m_pZone->m_index) ||
				(p->ch->m_pArea->m_index != df->m_pArea->m_index) ||
				!CheckInNearCellExt(p->ch, dpc))
			continue ;

		if(maxdamage <= p->m_damage)
		{
			maxdamage = p->m_damage;
			preference = p->ch;
		}

		//	p = p->m_next;
	}

	if(preference == NULL)
		return ;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(6653, -1, 0, 0, 1);
	if (!pItem)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		pItem->m_preferenceIndex = preference->m_index;
		dpc->m_pArea->DropItem(pItem, dpc);
		ItemDropMsg(rmsg, dpc, pItem);
		pItem->m_pArea->SendToCell(rmsg, dpc, true);
	}

	// 7. 게임 로그
	GAMELOG << init("WARCASTLE_TOKEN_DROP_DEAD_PC", dpc)
			<< "ITEM" << delim
			<< itemlog(pItem)
			<< end;
}

void DropWarCastleToken(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc || !npc)
		return ;

	if(!npc->m_proto->CheckFlag( (1 << 6) | (1 << 9) ))
		return ;

	CWarCastle* castle = CWarCastle::GetCastleObject(npc->m_pZone->m_index);
	if(!castle)
		return ;

	if(castle->GetState() == WCSF_NORMAL)
		return ;

	if(tpc->GetJoinFlag(castle->GetZoneIndex()) != WCJF_NONE)
	{
		// 확률 수정
		if(GetRandom(1, 100) <= 5)
		{
			CItem* pItem = gserver->m_itemProtoList.CreateItem(6653, -1, 0, 0, 1);
			if (!pItem)
				return ;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(pItem, npc);
				ItemDropMsg(rmsg, npc, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
			}

			// 7. 게임 로그
			GAMELOG << init("WARCASTLE_TOKEN_DROP", tpc)
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
		}
	}
}

void ProcFollowNPC(CNPC* npc)
{
	// 리더 플래그 0
	if (npc->m_proto->CheckLeaderBit(0) && npc->m_proto->m_family != -1)
	{
		// 주위에서 탱커 우선으로 최대 5인 HP/MP 회복
		int sx = npc->m_cellX - CELL_EXT;
		int ex = npc->m_cellX + CELL_EXT;
		int sz = npc->m_cellZ - CELL_EXT;
		int ez = npc->m_cellZ + CELL_EXT;

		int x, z;
		float dist[5] = {999.9f, 999.9f, 999.9f, 999.9f, 999.9f};
		CNPC* ret[5] = {NULL, NULL, NULL, NULL, NULL};

		int i = 0;
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
					// NPC만, 자신은 제외, 죽은넘 제외, 다른층 제외
					if (!IS_NPC(pChar) || pChar == npc || DEAD(pChar) || ABS(GET_YLAYER(npc) - GET_YLAYER(pChar)) >= 2)
						continue ;

					CNPC* tnpc = TO_NPC(pChar);

					// 동족이고
					if (tnpc->m_proto->m_family != npc->m_proto->m_family)
						continue ;

					// 기존과 타입검사 : 탱커, 자신의타입, 그외, 없음
					// 타입이 같으면 거리로 비교
					bool bReqSort = false;
					if (i < 5)
					{
						dist[i] = GetDistance(npc, tnpc);
						ret[i] = tnpc;
						i++;
						if (i == 5)
							bReqSort = true;
					}
					else
					{
						for (i = 0; i < 5; i++)
						{
							int resComp = AIComp(npc, tnpc, ret[i]);
							if (resComp < 0 || (resComp == 0 && dist[i] > GetDistance(npc, tnpc)))
							{
								dist[i] = GetDistance(npc, tnpc);
								ret[i] = tnpc;
								bReqSort = true;
								break;
							}
						}
					}

					// 거리 제일 먼 녀석이 앞에오게 정렬
					if (bReqSort)
					{
						int k;
						int j;
						int mindist;
						for (k = 0; k < 5 - 1; k++)
						{
							mindist = 0;
							for (j = 1; j < 5 - i; j++)
							{
								if (dist[j] < dist[mindist])
									mindist = j;
							}
							float tmpdist = dist[5 - k - 1];
							dist[5 - k - 1] = dist[mindist];
							dist[mindist] = tmpdist;
							CNPC* tmpNPC = ret[5 - k - 1];
							ret[5 - k - 1] = ret[mindist];
							ret[mindist] = tmpNPC;
						}
					}
				}
			}
		}

		for (i = 0; i < 5; i++)
		{
			if (ret[i])
			{
				ret[i]->m_hp = ret[i]->m_maxHP;
				ret[i]->m_mp = ret[i]->m_maxMP;
				CNetMsg::SP rmsg(new CNetMsg);
				CharStatusMsg(rmsg, ret[i], 0);
				ret[i]->m_pArea->SendToCell(rmsg, ret[i]);
			}
		}
	} // --- 리더 플래그 0
}

int AIComp(CNPC* base, CNPC* n1, CNPC* n2)
{
	if (n1 == NULL && n2 == NULL)
		return 0;
	if (n1 == NULL)
		return 1;
	if (n2 == NULL)
		return -1;

	int typeb = base->m_proto->m_aitype;
	int type1 = n1->m_proto->m_aitype;
	int type2 = n2->m_proto->m_aitype;

	if (type1 == type2)
		return 0;

	// 일반 몹은 최하 우선순위
	if (type1 != NPC_AI_NORMAL && type2 == NPC_AI_NORMAL)
		return -1;
	if (type1 == NPC_AI_NORMAL && type2 != NPC_AI_NORMAL)
		return 1;

	// 탱커 1순위
	if (type1 == NPC_AI_TANKER)			return -1;
	if (type2 == NPC_AI_TANKER)			return 1;
	// 동일타입 2순위
	if (type1 == typeb)					return -1;
	if (type2 == typeb)					return 1;
	// 그외 타입은 동일 순위
	return 0;
}

// of가 df를 죽이면 PK인지 검사
bool IsPK(CPC* of, CCharacter* df)
{
#ifdef MAL_DISABLE_PKPENALTY
	if( gserver->m_bDisablePKPaenalty )
		return false;
#endif // MAL_DISALBE_PKPENALTY

#ifdef FREE_PK_SYSTEM
	if( gserver->m_bFreePk )
		return false;
#endif // FREE_PK_SYSTEM

	bool bPvP = false;
	CPC* dpc = NULL;
	switch (df->m_type)
	{
	case MSG_CHAR_PC:
		dpc = TO_PC(df);
		bPvP = true;
		break;
	case MSG_CHAR_PET:
		dpc = TO_PET(df)->GetOwner();
		break;
	case MSG_CHAR_ELEMENTAL:
		dpc = TO_ELEMENTAL(df)->GetOwner();
		break;
	case MSG_CHAR_APET:
		dpc = TO_APET(df)->GetOwner();
		break;
	case MSG_CHAR_NPC:
		dpc = TO_NPC(df)->GetOwner();
		break;
	default:
		return false;
	}

	if (dpc == NULL)
		return false;

	// 기본적으로 죽는건 패널티가 폴스고, 페널티 주는건 트루
	// 뒈진넘이 pk모드 이거나 카오면 둘다 트루
	// 뒈진넘이 정방 리스트에 있고 카오가 아니고 공격자도 카오가 아니면 카오성향 수치를 계산하지 않는다
	// pk 패널티를 주는것 뿐만 아니라 성향회복에도 관계되므로 성향 회복이나 패널티등 어느것에라도 걸리면 true

	// of의 정당방위 리스트에 dpc가 있는가 : 정당방위 - 리스트에서 삭제후 Point 페널티 없음
	CRaChar* raList = of->m_raList;
	while (raList)
	{
		if (raList->m_raTarget == dpc)
		{
			// 카오가 일반을 정당방위로 죽일때 패널티를 위한 of
			// 일반이 카오를 정당방위로 죽일때 패널티 및 성향 상승을 위한 dpc
			// 둘다 일반이고 죽은넘이 공격했을때 m_bAttacker
			if (raList->m_bAttacker && !dpc->IsChaotic())
				return false;

			break;
		}
		raList = raList->m_next;
	}

#ifdef LC_TLD
	// 둘다 카오가 아니고 칼키고 있으면 태국은 성향 수치 변화없음
	if( dpc->IsSetPlayerState(PLAYER_STATE_PKMODE) && of->IsSetPlayerState(PLAYER_STATE_PKMODE) && !of->IsChaotic() && !dpc->IsChaotic() )
		return false;
#endif

	// 길드전
	if (of->m_guildInfo && (of->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING) &&
			dpc->m_guildInfo && (dpc->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
	{
		if (of->m_guildInfo->guild()->battleIndex() == dpc->m_guildInfo->guild()->index() &&
				dpc->m_guildInfo->guild()->battleIndex() == of->m_guildInfo->guild()->index())
			return false;
	}

#ifdef CHANGE_WARCASTLE_SETTING
	// 서로 적대 관계(공성, 수성)이면 페널티가 없다.
	CPC* pDf = TO_PC(df);
	int nZoneIdx = -1;
	CWarCastle* castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if (castle && castle->GetState() != WCSF_NORMAL)
	{
		nZoneIdx = ZONE_DRATAN;
	}
	else
	{
		castle = CWarCastle::GetCastleObject(ZONE_MERAC);
		if (castle && castle->GetState() != WCSF_NORMAL)
		{
			nZoneIdx = ZONE_MERAC;
		}
	}
	if ( pDf && nZoneIdx > 0 )
	{
		if ( (IS_DEFENSE_TEAM(of->GetJoinFlag(nZoneIdx)) && IS_ATTACK_TEAM(pDf->GetJoinFlag(nZoneIdx))) ||					// 수성 vs 공성
				(IS_ATTACK_TEAM(of->GetJoinFlag(nZoneIdx)) && pDf->GetJoinFlag(nZoneIdx) != WCJF_NONE) )						// 공성 vs 수성 or 공성
		{
			return false;
		}
#ifdef CHECK_CASTLE_AREA
		if ((of->m_pZone->m_index == nZoneIdx && pDf->m_pZone->m_index == nZoneIdx ) &&										// 공성존이고,공성지역내에 있고, 공,수성(of) vs 3세력(df)
				(of->GetMapAttr() & MATT_WAR || of->m_pZone->IsWarZone((int)of->m_pos.m_x, (int)of->m_pos.m_z)) &&
				(df->GetMapAttr() & MATT_WAR || df->m_pZone->IsWarZone((int)df->m_pos.m_x, (int)df->m_pos.m_z)) &&
				of->GetJoinFlag(nZoneIdx) != WCJF_NONE && pDf->GetJoinFlag(nZoneIdx) == WCJF_NONE)
#else // CHECK_CASTLE_AREA
		if ((of->m_pZone->m_index == nZoneIdx && pDf->m_pZone->m_index == nZoneIdx ) &&
				(of->GetMapAttr() == pDf->GetMapAttr() && of->GetMapAttr() & MATT_WAR && of->GetJoinFlag(nZoneIdx) != WCJF_NONE && pDf->GetJoinFlag(nZoneIdx) == WCJF_NONE) )
#endif // CHECK_CASTLE_AREA
		{
			return false;
		}
	}
#else // CHANGE_WARCASTLE_SETTING
	// 공성 도중 사망은 패널티 없음
	CWarCastle* castle = CWarCastle::GetCastleObject(of->m_pZone->m_index);
	if (castle)
	{
		if (castle->GetState() != WCSF_NORMAL)
		{
#ifdef CHECK_CASTLE_AREA
			if ( (of->GetMapAttr() & MATT_WAR || of->m_pZone->IsWarZone((int)of->m_pos.m_x, (int)of->m_pos.m_z)) &&
					(df->GetMapAttr() & MATT_WAR || df->m_pZone->IsWarZone((int)df->m_pos.m_x, (int)df->m_pos.m_z)) &&
					(of->GetJoinFlag(of->m_pZone->m_index) != WCJF_NONE ))
				return false;
#else
			if (of->GetMapAttr() & MATT_WAR &&
					df->GetMapAttr() & MATT_WAR)
				return false;
#endif // CHECK_CASTLE_AREA
		}
	}
#endif // CHANGE_WARCASTLE_SETTING
//#endif

	/////////////////////////////////////////////
	// BANGWALL : 2005-07-18 오전 11:27:24
	// Comment : freepkzone 패널티 없음
	// 공격자와 방어자가 모두 freepkzone에 있으면 pkpenalty 없음
	if( of->GetMapAttr() == df->GetMapAttr() && of->GetMapAttr() & MATT_FREEPKZONE)
		return false;

	return true;
}
// npc attackList중 어택 제한 Pulse 초과한 경우 어택리스트에서 삭제
// hate 수치 가장 높은 pc 반환
//
CCharacter* CheckAttackPulse(CNPC* npc)
{
	CAttackChar* target;
	CAttackChar* targetNext = npc->m_attackList;

	int hate = 0;
	CCharacter* retHate = NULL;
	int lowhp = 0x7fffffff;
	int lowhp_hate = 0;
	CCharacter* retLowHP = NULL;

	int checkPulse;

	while ((target = targetNext))
	{
		targetNext = targetNext->m_next;

		// 미네랄, 농작물, 에너지
		if (npc->m_proto->CheckFlag(NPC_MINERAL | NPC_CROPS | NPC_ENERGY))
			checkPulse = NPC_PRODUCE_DELETE_PULSE;
		// 그외 기타 NPC
		else
			checkPulse = NPC_ATTACK_DELETE_PULSE;

		// 타겟이 때린 데미지가 있고 Pulse가 초과한 경우 리스트에서 삭제
		if (target->m_damage > 0 && gserver->m_pulse - target->m_targetPulse > checkPulse)
		{
			// target의 attackList에서 npc 삭제
			CAttackChar* tmp;
			CAttackChar* tmpNext = target->ch->m_attackList;
			while ((tmp = tmpNext))
			{
				tmpNext = tmpNext->m_next;

				if (tmp->ch == npc)
				{
					REMOVE_FROM_BILIST(tmp, target->ch->m_attackList, m_prev, m_next);
					delete tmp;
					tmp = NULL;
				}
			}

			REMOVE_FROM_BILIST(target, npc->m_attackList, m_prev, m_next);
			delete target;
			target = NULL;
			continue;
		}

		// 050315 : bs : 타겟 볼수 없으면 스킵
		if (!target)
			continue ;
		if (!npc->CanVisible(target->ch))
			continue ;

		if ((target->ch->m_assist.m_state & AST_FAKEDEATH))
			continue ;

		// 시야범위안에 없으면 타겟이 될 수 없다.
		if (!CheckInNearCellExt(npc, target->ch))
			continue ;

		// 층이 다르고 높이가 5미터 이상이면 무시
		if (ABS(GET_YLAYER(npc) - GET_YLAYER(target->ch)) > 1 && ABS(GET_H(npc) - GET_H(target->ch)) > 5.0f)
			continue ;

		// 피스존 안에서는 타겟이 될 수 없다
		if (target->ch->IsInPeaceZone(false))
			continue ;

		if (hate <= target->m_targetHate && target->m_targetHate != 0)
		{
			hate = target->m_targetHate;
			retHate = target->ch;
		}
		if (lowhp > target->ch->m_hp && target->m_targetHate != 0)
		{
			lowhp = target->ch->m_hp;
			lowhp_hate = target->m_targetHate;
			retLowHP = target->ch;
		}
	}

	switch (npc->m_proto->m_aitype)
	{
	case NPC_AI_TANKER:
		// 자신의 체력이 20% 미만이면 hate 높은 대상으로부터 도망 : flag 2
		if ( npc->m_proto->CheckAIBit(2) && npc->m_hp < npc->m_maxHP * 20 / 100 )
			return retHate;

		// 피가 제일 적은 대상이 최대 hate를 갖는 대상의 hate 절반 이상이면 피 적은 녀석을 공격 : flag 0
		if ( npc->m_proto->CheckAIBit(0) && hate  / 2 < lowhp_hate )
		{
			// 체력이 적은 대상과 거리가 10미터 이상 차이나면 hate 높은 대상을 공격 : flag 1
			if ( npc->m_proto->CheckAIBit(1) && GetDistance(retLowHP, npc) >= 10.0f )
				return retHate;

			// 체력 낮은 적과 hate 높은 적이 HP가 같으면 hate 높은 적
			if (retHate && retLowHP && retHate->m_hp == retLowHP->m_hp)
				return retHate;

			return retLowHP;
		}

		return retHate;

	case NPC_AI_NORMAL:
	case NPC_AI_DAMAGEDEALER:
	case NPC_AI_HEALER:
	default:
		return retHate;
	}
}

int CalcNewDamage(CCharacter* of, CCharacter* df, int damage)
{
	if( IS_PC(of) && IS_PC(df) )
	{
		CPC* of_pc = TO_PC(of);
		CPC* df_pc = TO_PC(df);

		if(of_pc->IsChaotic() || df_pc->IsChaotic())
		{
			//공격자 처리
			if(of_pc->IsChaotic() == true)
			{
				damage = damage - (damage * of_pc->m_pkPenalty / 1000 / 100 * (-1.5));
			}
			else
			{
				damage = damage + (damage * of_pc->m_pkPenalty / 1000 / 100 * 3.5);
			}
		}

		//방어구강화에 따른 수정
		{
			damage = damage - (df_pc->m_wearInventory.getWearItemPlusSumCount() * damage / (df_pc->m_wearInventory.getWearItemPlusSumCount() + 310));
		}
	}
	
	return damage;
}

void CalcNewHitProb(CCharacter* of, CCharacter* df, float& hit, float& avoid)
{
	if( IS_PC(of) && IS_PC(df) )
	{
		CPC* of_pc = TO_PC(of);
		CPC* df_pc = TO_PC(df);

		//무기에 따른 명중률 계산
		CItem* item_weapon = of_pc->m_wearInventory.getWearItem(WEARING_WEAPON);
		if(item_weapon != NULL)
		{
			hit = hit + ( item_weapon->getPlus() * ( (of_pc->m_level + 900) / df_pc->m_level ) );
		}

		//카오 모드 일 경우의 PVP 회피 감소
		if(df_pc->IsChaotic() == true)
		{
			avoid = avoid + ((df_pc->m_pkPenalty / (of_pc->m_level + df_pc->m_level)) - of_pc->m_level * 3);
		}
	}
}
bool checkPvPProtect( CPC* pc, CCharacter* tch )
{
	CPC* tpc = NULL;
	if(IS_PC(tch))
		tpc = TO_PC(tch);
	else if(IS_APET(tch))
		tpc = TO_APET(tch)->GetOwner();
	else if(IS_PET(tch))
		tpc = TO_PET(tch)->GetOwner();
	else if(IS_ELEMENTAL(tch))
		tpc = TO_ELEMENTAL(tch)->GetOwner();
	else if(IS_NPC(tch))
		tpc = TO_NPC(tch)->GetOwner();

	if(tpc == NULL)
		return true;

	if(tpc == pc)
		return true;

	if(tpc->m_assist.FindBySkillIndex(PVP_PROTECT_SKILL_INDEX) != 0)
	{
		//공성중인 경우에
		if(tpc->GetJoinFlag(tpc->m_pZone->m_index) != 0)
		{
			if ( tpc->GetMapAttr() & MATT_WAR || tpc->m_pZone->IsWarZone(GET_X(tpc), GET_Z(tpc)) == true ) 
			{
				return true;
			}
			else
			{
				//이외에는 공격 불가
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_DO_NOT_ATTACK_PROTECT_PVP);
				SEND_Q(rmsg, pc->m_desc);
				return false;
			}
		}
		else
		{
			switch(tpc->m_pArea->m_zone->m_index)
			{
			case ZONE_AKAN_TEMPLE:
			case ZONE_EGEHA_PK:
			case ZONE_PK_TOURNAMENT:
			case ZONE_EXTREME_CUBE:
			case ZONE_ROYAL_RUMBLE:
			case ZONE_RVR:
				break;
			default:
				// 공격 불가
				if ( !(tpc->GetMapAttr() & MATT_FREEPKZONE) )
				{
					//아레나등 지역에서는 공격 불가
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_DO_NOT_ATTACK_PROTECT_PVP);
					SEND_Q(rmsg, pc->m_desc);
					return false;
				}
				break;
			}
		}
	}
	return true;
}
