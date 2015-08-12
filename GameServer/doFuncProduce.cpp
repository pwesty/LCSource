#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"
#include "WarCastle.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

int GetGradeProductItem(int itemdbindex)
{
	int grade = -1;
	switch (itemdbindex)
	{
	case 156:	// 스톤 온			E
	case 161:	// E등급 원소		E
	case 199:	// 크락의 노란잎	E
		grade = 0;
		break;

	case 160:	// D등급 원소		D
	case 155:	// 스톤 리스		D
	case 198:	// 크락의 줄기		D
		grade = 1;
		break;

	case 159:	// C등급 원소		C
	case 154:	// 스톤 웨버		C
	case 197:	// 크락의 파란잎	C
		grade = 2;
		break;

	case 158:	// B등급 원소		B
	case 153:	// 스톤 비스트		B
	case 163:	// 크락의 가시		B
		grade = 3;
		break;

	case 152:	// 스톤 웰스던		A
	case 157:	// A등급 원소		A
	case 162:	// 크락의 꽃		A
		grade = 4;
		break;
	}

	return grade;
}

//////////////////////////////
// 선택 생산
void do_SelectProduce(CPC* ch, CNetMsg::SP& msg)
{
	if (DEAD(ch))
		return ;

	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE))
		return;

	int npcindex;
	int itemdbindex;

	msg->MoveFirst();

	RefMsg(msg) >> npcindex
				>> itemdbindex;

	// 근처의 NPC 찾기
	CNPC* npc = TO_NPC(ch->m_pArea->FindCharInCell(ch, npcindex, MSG_CHAR_NPC, false));
	if (!npc)
		return ;

	// NPC의 a_product0 ~ 4에서 찾기
	int i;
	for (i = 0; i < MAX_PRODUCTS_FROM_NPC; i++)
	{
		if (npc->m_proto->m_product[i] == itemdbindex)
			break;
	}

	if (i == MAX_PRODUCTS_FROM_NPC)
		return ;

	// 속도 검사
	ch->m_hackProduceCount--;
	ch->m_ProduceClientTime += gserver->m_pulse - ch->m_lastProducePulse + PULSE_HACK_ATTACK_THRESHOLD;

	if (ch->m_hackProduceCount < 1)
	{
		if( ch->m_ProduceClientTime < PULSE_PRODUCE_DELAY * PULSE_HACK_ATTACK_COUNT)
		{
			GAMELOG << init("HACK PRODUCE SPEED", ch)
					<< (gserver->m_pulse - ch->m_lastProducePulse)
					<< end;
			ch->m_desc->IncreaseHackCount(0);
			return;
		}
		ch->m_hackProduceCount = PULSE_HACK_ATTACK_COUNT;	// 샌상메세지 온 수 저장
		ch->m_ProduceClientTime = 0; // 클라이언트가 보낸 생산시간
	}

	ch->m_lastProducePulse = gserver->m_pulse;

	// 거리 검사
	if (GetDistance(ch, npc) > npc->m_proto->m_attackArea + 0.5)
	{
		GAMELOG << init("HACK PRODUCE DIST", ch)
				<< GetDistance(ch, npc)
				<< end;
		// ch->m_desc->IncreaseHackCount(6);
		return ;
	}

	// 아이템 DB 인덱스 검사
	int npcflag = -1;			// NPC 종류
	int needitemtype = -1;		// 도구 종류
	int needskilltype = -1;		// 스킬 종류
	switch (itemdbindex)
	{
	case 152:	// 스톤 웰스던
	case 153:	// 스톤 비스트
	case 154:	// 스톤 웨버
	case 155:	// 스톤 리스
	case 156:	// 스톤 온
		if (!npc->m_proto->CheckFlag(NPC_MINERAL))
			return ;
		npcflag = NPC_MINERAL;
		needitemtype = IWEAPON_MINING;
		needskilltype = SSKILL_MINING;
		break;

	case 157:	// A등급 원소
	case 158:	// B등급 원소
	case 159:	// C등급 원소
	case 160:	// D등급 원소
	case 161:	// E등급 원소
		if (!npc->m_proto->CheckFlag(NPC_ENERGY))
			return ;
		npcflag = NPC_ENERGY;
		needitemtype = IWEAPON_CHARGE;
		needskilltype = SSKILL_CHARGE;
		break;

	case 162:	// 크락의 꽃
	case 163:	// 크락의 가시
	case 197:	// 크락의 파란잎
	case 198:	// 크락의 줄기
	case 199:	// 크락의 노란잎
		if (!npc->m_proto->CheckFlag(NPC_CROPS))
			return ;
		npcflag = NPC_CROPS;
		needitemtype = IWEAPON_GATHERING;
		needskilltype = SSKILL_GATHERING;
		break;

	default:
		return ;
	}

	// 등급 구하기
	int grade = GetGradeProductItem(itemdbindex);
	if (grade == -1)
		return ;

	// 도구 검사
	if (!ch->m_wearInventory.wearItemInfo[WEARING_WEAPON])
	{
		// 아예 무기위치에 암것두 없음
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_NOTOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* tool = ch->m_wearInventory.wearItemInfo[WEARING_WEAPON];

	// 생산 도구 타입 검사
	bool bProduceTool = false;
	switch (tool->m_itemProto->getItemTypeIdx())
	{
	case ITYPE_WEAPON:
		switch (tool->m_itemProto->getItemSubTypeIdx())
		{
		case IWEAPON_MINING:
		case IWEAPON_GATHERING:
		case IWEAPON_CHARGE:
			bProduceTool = true;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
	if (!bProduceTool)
	{
		// 생산 도구가 아님
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_WEAR_TOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if (tool->m_itemProto->getItemSubTypeIdx() != needitemtype)
	{
		// 해당 생산 도구가 아님
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_MATCH_TOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 부스터 옵션 체크
	int bBoost  = 0;
	if(tool->getFlag() & FLAG_ITEM_BOOSTER_ADDED)
		bBoost = 100;
	else if(tool->getFlag() & FLAG_ITEM_SILVERBOOSTER_ADDED)
		bBoost = 140;
	else if(tool->getFlag() & FLAG_ITEM_GOLDBOOSTER_ADDED)
		bBoost = 200;
	else if(tool->getFlag() & FLAG_ITEM_PLATINUMBOOSTER_ADDED)
		bBoost = 300;

	const int produceTax = 30;		// 생산당 필요 나스

	// 부스터는 금액 검사 안함, 사유지에서만
	if (!bBoost && npc->GetMapAttr() & MATT_PRODUCT_PRIVATE)
	{
		if (ch->m_inventory.getMoney() < produceTax)
		{
			// 돈 없어서 불가능
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_NO_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

// [101216: selo] 보상 아이템 드롭 수정
	if( !ch->CheckInvenForProduceNoramlPrize(itemdbindex, grade, bBoost) )
	{
		return;
	}

	// 부스터 도구는 내구도도 안 떨어진다
	// 생산 도구 내구도 검사
	if (!bBoost && tool->m_itemProto->getItemMaxUse() != -1 &&  tool->getUsed() <= 0)
	{
		// 내구 다 되었을 때
		ch->m_wearInventory.RemoveItem((int)tool->getWearPos());

		{
			// 남들한테 벗기기
			CNetMsg::SP rmsg(new CNetMsg);
			WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_MAXUSE_TOOL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return;
	}

	// 생산 가능 인원 체크
	int nTarget = 0;
	CAttackChar* target = npc->m_attackList;
	bool bProduce = false;		// 이번에 생산 메시지보낸 유저는 생산중이다

	while (target)
	{
		nTarget++;

		if (target->ch == ch)
			bProduce = true;

		target = target->m_next;
	}

	// 생산메시지 보낸유저가 이미 생산중이라면 인원체크 안한다.
	if (!bProduce)
	{
		// 생산 가능 인원 초과
		if (nTarget >= MAX_ACCEPTABLE)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_MAX_ACCEPTABLE);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 진행중인 Quest 중 죽은 npc로 진행중이면 UpdateData
	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		// 수집 퀘스트
		switch (pQuest->GetQuestType0())
		{
		case QTYPE_KIND_COLLECTION:
		case QTYPE_KIND_MINING_EXPERIENCE:
		case QTYPE_KIND_GATHERING_EXPERIENCE:
		case QTYPE_KIND_CHARGE_EXPERIENCE:
			pQuest->QuestUpdateData(ch, npc);
			break;
		default:
			break;
		}
	}

	// 스킬 검사
	// 인덱스 큰 것이 더 높은 등급으로 하여 상위 레벨을 선택
	CSSkillNode* node = ch->m_sSkillList.m_head;
	CSSkill* sskill = NULL;

	// 부스터 도구는 스킬 검사 안한다
	// 부스터 도구는 내구도도 안 떨어진다
	if (!bBoost)
	{
		while (node)
		{
			if (node->m_sskill->m_proto->m_type == needskilltype)
			{
				if (sskill == NULL || sskill->m_level <= node->m_sskill->m_level)
					sskill = node->m_sskill;
			}
			node = node->m_next;
		}
		if (sskill == NULL || sskill->m_level < 0 || sskill->m_level >= SSKILL_MAX_LEVEL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_NO_SSKILL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 아이템 이용 횟수 감소
		if (tool->m_itemProto->getItemMaxUse() > 0)
		{
			tool->setUsed(tool->getUsed() - 1);

			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeUpdateWearItemUsed(rmsg, tool->getWearPos(), tool->getVIndex(), tool->getUsed());
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			// 내구 다 되었을 때
			// 벗기기
			ch->m_wearInventory.RemoveItem((int)tool->getWearPos());
			{
				// 남들한테 벗기기
				CNetMsg::SP rmsg(new CNetMsg);
				WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
				ch->m_pArea->SendToCell(rmsg, ch, false);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_PRODUCE_MAXUSE_TOOL);
				SEND_Q(rmsg, ch->m_desc);
			}

			return;
		}
	}

	// npc 체력 감소
	npc->m_hp -= tool->m_itemProto->getItemNum0();

	if (npc->m_hp <= 0)
		npc->m_hp = 0;

	// 어택리스트에 추가
	AddAttackList(ch, npc, 0);

	// 공격 리스트에서 데미지 갱신
	target = npc->m_attackList;

	while (target)
	{
		if (target->ch == ch)
		{
			target->m_damage += tool->m_itemProto->getItemNum0();
			// 생산의 경우는 펄스값을 매번 셋팅해준다.
			target->m_targetPulse = gserver->m_pulse;
			break;
		}
		target = target->m_next;
	}

	// 5 때릴 때 마다
	if (target->m_damage >= 5)
	{
		CItem* item = NULL;

		// 부스터인가?
		if (bBoost)
		{
			// 부스터 아이템
			// 부스터 도구는 패널티 없고 이벤트와 무관

			// 대미지 리셋
			target->m_damage = 0;

			// 등급별 획득 개수
			LONGLONG countOfGrade[5] = {150, 86, 55, 38, 30};

			// TODO : 임시 조정
			item = gserver->m_itemProtoList.CreateItem(itemdbindex, -1, 0, 0, countOfGrade[grade] / 2 * bBoost / 100 );
			if (item)
			{
				// 내구 다 되었을 때
				// 벗기기
				ch->m_wearInventory.RemoveItem((int)tool->getWearPos());
				{
					// 남들한테 벗기기
					CNetMsg::SP rmsg(new CNetMsg);
					WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
					ch->m_pArea->SendToCell(rmsg, ch, false);
				}

			}

			npc->m_hp = 0;
		}
		else
		{
			// 일반 아이템

			// 대미지 줄이기
			target->m_damage -= 5;

			// 현재 sskill에서 특정 등급을 얻을 확률 구함
			int prob = 0;
			switch (grade)
			{
			case 0:
				prob = sskill->m_proto->m_num0[sskill->m_level - 1];
				break;	// E
			case 1:
				prob = sskill->m_proto->m_num1[sskill->m_level - 1];
				break;	// D
			case 2:
				prob = sskill->m_proto->m_num2[sskill->m_level - 1];
				break;	// C
			case 3:
				prob = sskill->m_proto->m_num3[sskill->m_level - 1];
				break;	// B
			case 4:
				prob = sskill->m_proto->m_num4[sskill->m_level - 1];
				break;	// A
			}

			// 무제한 생산 페널티
			if (npc->m_proto->CheckFlag(NPC_ETERNAL))
				prob = prob * ETERNAL_PENALTY_PROB / 10000;

			if (npc->GetMapAttr() & MATT_PRODUCT_PUBLIC)
			{
				// 공유지 페널디
				prob = prob * PUBLIC_PENALTY_PROB / 10000;
			}

			prob = prob * gserver->m_bDoubleProducePercent / 10000;

			// 성공 결정
			if (GetRandom(1, 10000) <= prob)
			{
				LONGLONG count = 1;
				// 더블 이벤트
				if (gserver->m_bDoubleEvent)
					count = gserver->m_bDoubleProduceNum;

				item = gserver->m_itemProtoList.CreateItem(itemdbindex, -1, 0, 0, count);

				if (npc->GetMapAttr() & MATT_PRODUCT_PRIVATE)
				{
					if (ch->m_inventory.getMoney() >= produceTax)
					{
						ch->m_inventory.decreaseMoney(produceTax);
					}
					gserver->AddTaxProduceCastle(produceTax / 3);
				}
			}
		}

		if (item && item->m_itemProto->getItemTypeIdx() == ITYPE_ETC && item->m_itemProto->getItemSubTypeIdx() == IETC_PRODUCT)
		{
			if (ch->m_inventory.addItem(item))
			{
				// Item LOG
				GAMELOG << init("ITEM_PICK_PRODUCE_SEL", ch)
						<< itemlog(item)
						<< end;

				switch (item->m_itemProto->getItemIndex())
				{
				case 152:
					STATISTICS(152, item->Count());
					break;
				case 153:
					STATISTICS(153, item->Count());
					break;
				case 154:
					STATISTICS(154, item->Count());
					break;
				case 155:
					STATISTICS(155, item->Count());
					break;
				case 156:
					STATISTICS(156, item->Count());
					break;
				case 157:
					STATISTICS(157, item->Count());
					break;
				case 158:
					STATISTICS(158, item->Count());
					break;
				case 159:
					STATISTICS(159, item->Count());
					break;
				case 160:
					STATISTICS(160, item->Count());
					break;
				case 161:
					STATISTICS(161, item->Count());
					break;
				case 162:
					STATISTICS(162, item->Count());
					break;
				case 163:
					STATISTICS(163, item->Count());
					break;
				case 197:
					STATISTICS(197, item->Count());
					break;
				case 198:
					STATISTICS(198, item->Count());
					break;
				case 199:
					STATISTICS(199, item->Count());
					break;
				}
			}
			else
			{
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = npc->m_pArea->DropItem(item, npc);
				if (!item)
					return;

				item->m_preferenceIndex = ch->m_index;
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, npc, true);
				}
			}

			{
				// 생산 성공시 해당하는 종류에 따라 이펙트 메세지 전송
				CNetMsg::SP rmsg(new CNetMsg);
				switch (npcflag)
				{
				case NPC_MINERAL:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_MINING);
					break;
				case NPC_ENERGY:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_GATHERING);
					break;
				case NPC_CROPS:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_CHARGE);
					break;
				}
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
	}

	// 지속
	if (npc->m_proto->CheckFlag(NPC_ETERNAL))
	{
		npc->m_hp = ch->m_maxHP;
	}

	{
		// 이펙트 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		switch (npcflag)
		{
		case NPC_MINERAL:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_MINING);
			break;
		case NPC_ENERGY:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_CHARGE);
			break;
		case NPC_CROPS:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_GATHERING);
			break;
		}
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	// 죽었을때
	if (DEAD(npc))
	{
		// 지속
		if (npc->m_proto->CheckFlag(NPC_ETERNAL))
		{
			npc->m_hp = ch->m_maxHP;
		}

		// 비지속
		else
		{
			// 아이템 드롭
			for (int loopcount = 0; loopcount < MAX_NPC_DROPITEM_LOOP; loopcount++)
			{
				// 1. 드롭할 수 있는 수 범위에서 아이템 선정
				// 2. 그 아이팀의 드롭확률로 드롭여부 결정
				// 3. 드롭
				if (npc->m_proto->m_itemCount > 0)
				{
					CItem* dropItem = NULL;
					int tableindex = GetRandom(0, MAX_NPC_DROPITEM - 1);

					if (npc->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= npc->m_proto->m_itemPercent[tableindex])
						dropItem = npc->m_pArea->DropItem(npc->m_proto->m_item[tableindex], npc, 0, 0, 1, true);

					if (dropItem)
					{
						// Drop Msg 보내기
						dropItem->m_preferenceIndex = -1;
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, npc, dropItem);
						npc->m_pArea->SendToCell(rmsg, npc);
					}
				}
			}

			// 어택리스트에서 삭제
			DelAttackList(npc);
			
			// npc 삭제
			npc->SendDisappearAllInCell( false );
			ch->m_pArea->CharFromCell(npc, true);
			ch->m_pArea->DelNPC(npc);
		}
	}
}

//////////////
// 무작위 생산
void do_RandomProduce(CPC* ch, CNetMsg::SP& msg)
{
	if (DEAD(ch))
		return ;

	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE))
		return;

	int npcindex;

	msg->MoveFirst();

	RefMsg(msg) >> npcindex;

	// 근처의 NPC 찾기
	CNPC* npc = TO_NPC(ch->m_pArea->FindCharInCell(ch, npcindex, MSG_CHAR_NPC, false));
	if (!npc)
		return ;

	// 속도 검사
	ch->m_hackProduceCount--;
	ch->m_ProduceClientTime += gserver->m_pulse - ch->m_lastProducePulse + PULSE_HACK_ATTACK_THRESHOLD;

	if (ch->m_hackProduceCount < 1)
	{
		if( ch->m_ProduceClientTime < PULSE_PRODUCE_DELAY * PULSE_HACK_ATTACK_COUNT)
		{
			GAMELOG << init("HACK PRODUCE SPEED", ch)
					<< (gserver->m_pulse - ch->m_lastProducePulse)
					<< end;

			ch->m_desc->IncreaseHackCount(0);
			return;
		}
		ch->m_hackProduceCount = PULSE_HACK_ATTACK_COUNT;	// 샌상메세지 온 수 저장
		ch->m_ProduceClientTime = 0; // 클라이언트가 보낸 생산시간
	}

	ch->m_lastProducePulse = gserver->m_pulse;

	// 거리 검사
	if (GetDistance(ch, npc) > npc->m_proto->m_attackArea + 0.5)
	{
		GAMELOG << init("HACK PRODUCE DIST", ch)
				<< GetDistance(ch, npc)
				<< end;
		// ch->m_desc->IncreaseHackCount(6);
		return ;
	}

	// 아이템 DB 인덱스 검사
	int npcflag = -1;			// NPC 종류
	int needitemtype = -1;		// 도구 종류
	int needskilltype = -1;		// 스킬 종류

	if (npc->m_proto->CheckFlag(NPC_MINERAL))
	{
		npcflag = NPC_MINERAL;
		needitemtype = IWEAPON_MINING;
		needskilltype = SSKILL_MINING;
	}
	else if (npc->m_proto->CheckFlag(NPC_CROPS))
	{
		npcflag = NPC_CROPS;
		needitemtype = IWEAPON_GATHERING;
		needskilltype = SSKILL_GATHERING;
	}
	else if (npc->m_proto->CheckFlag(NPC_ENERGY))
	{
		npcflag = NPC_ENERGY;
		needitemtype = IWEAPON_CHARGE;
		needskilltype = SSKILL_CHARGE;
	}
	else
		return ;

	// 도구 검사
	if (!ch->m_wearInventory.wearItemInfo[WEARING_WEAPON])
	{
		// 아예 무기위치에 암것두 없음
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_NOTOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* tool = ch->m_wearInventory.wearItemInfo[WEARING_WEAPON];

	// 생산 도구 타입 검사
	bool bProduceTool = false;
	switch (tool->m_itemProto->getItemTypeIdx())
	{
	case ITYPE_WEAPON:
		switch (tool->m_itemProto->getItemSubTypeIdx())
		{
		case IWEAPON_MINING:
		case IWEAPON_GATHERING:
		case IWEAPON_CHARGE:
			bProduceTool = true;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
	if (!bProduceTool)
	{
		// 생산 도구가 아님
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_WEAR_TOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if (tool->m_itemProto->getItemSubTypeIdx() != needitemtype)
	{
		// 해당 생산 도구가 아님
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_MATCH_TOOL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 부스터 아이템 사용 불가
	if (tool->getFlag() & FLAG_ITEM_BOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_SILVERBOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_GOLDBOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_PLATINUMBOOSTER_ADDED
	   )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_CANNT_BOOST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	const int produceTax = 30;		// 생산당 필요 나스

	// 부스터는 금액 검사 안함, 사유지에서만
	if (npc->GetMapAttr() & MATT_PRODUCT_PRIVATE)
	{
		if (ch->m_inventory.getMoney() < produceTax)
		{
			// 돈 없어서 불가능
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_NO_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

// [101216: selo] 보상 아이템 드롭 수정
	if( !ch->CheckInvenForProduceRandomPrize() )
	{
		return;
	}

	// 생산 도구 내구도 검사
	if (tool->m_itemProto->getItemMaxUse() != -1 &&  tool->getUsed() <= 0)
	{
		// 내구 다 되었을 때
		// 벗기기
		ch->m_wearInventory.RemoveItem((int)tool->getWearPos());

		{
			// 남들한테 벗기기
			CNetMsg::SP rmsg(new CNetMsg);
			WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_MAXUSE_TOOL);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}

	// 생산 가능 인원 체크
	int nTarget = 0;
	CAttackChar* target = npc->m_attackList;
	bool bProduce = false;		// 이번에 생산 메시지보낸 유저는 생산중이다

	while (target)
	{
		nTarget++;

		if (target->ch == ch)
			bProduce = true;

		target = target->m_next;
	}

	// 생산메시지 보낸유저가 이미 생산중이라면 인원체크 안한다.
	if (!bProduce)
	{
		// 생산 가능 인원 초과
		if (nTarget >= MAX_ACCEPTABLE)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_MAX_ACCEPTABLE);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 진행중인 Quest 중 죽은 npc로 진행중이면 UpdateData
	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		// 수집 퀘스트
		switch (pQuest->GetQuestType0())
		{
		case QTYPE_KIND_COLLECTION:
		case QTYPE_KIND_MINING_EXPERIENCE:
		case QTYPE_KIND_GATHERING_EXPERIENCE:
		case QTYPE_KIND_CHARGE_EXPERIENCE:
			pQuest->QuestUpdateData(ch, npc);
			break;
		default:
			break;
		}
	}

	// 스킬 검사
	// 인덱스 큰 것이 더 높은 등급으로 하여 상위 레벨을 선택
	CSSkillNode* node = ch->m_sSkillList.m_head;
	CSSkill* sskill = NULL;

	while (node)
	{
		if (node->m_sskill->m_proto->m_type == needskilltype)
		{
			if (sskill == NULL || sskill->m_level <= node->m_sskill->m_level)
				sskill = node->m_sskill;
		}
		node = node->m_next;
	}
	if (sskill == NULL || sskill->m_level < 0 || sskill->m_level >= SSKILL_MAX_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PRODUCE_NO_SSKILL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 아이템 이용 횟수 감소
	if (tool->m_itemProto->getItemMaxUse() > 0)
	{
		tool->setUsed(tool->getUsed() - 1);

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateWearItemUsed(rmsg, tool->getWearPos(), tool->getVIndex(), tool->getUsed());
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 내구 다 되었을 때
		ch->m_wearInventory.RemoveItem((int)tool->getWearPos());
		{
			// 남들한테 벗기기
			CNetMsg::SP rmsg(new CNetMsg);
			WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_PRODUCE_MAXUSE_TOOL);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}

	// npc 체력 감소
	npc->m_hp -= tool->m_itemProto->getItemNum0();
	if (npc->m_hp <= 0)
		npc->m_hp = 0;

	// 어택리스트에 추가
	AddAttackList(ch, npc, 0);

	// 공격 리스트에서 데미지 갱신
	target = npc->m_attackList;

	while (target)
	{
		if (target->ch == ch)
		{
			target->m_damage += tool->m_itemProto->getItemNum0();
			// 생산의 경우는 펄스값을 매번 셋팅해준다.
			target->m_targetPulse = gserver->m_pulse;
			break;
		}
		target = target->m_next;
	}

	// 5 때릴 때 마다
	if (target->m_damage >= 5)
	{
		CItem* item = NULL;

		// 대미지 줄이기
		target->m_damage -= 5;

		// 등급별 확률 구하기
		int probProduct[MAX_PRODUCTS_FROM_NPC];
		int itemIndexProduct[MAX_PRODUCTS_FROM_NPC];
		memset(probProduct, 0, sizeof(int) * MAX_PRODUCTS_FROM_NPC);
		memset(itemIndexProduct, 0, sizeof(int) * MAX_PRODUCTS_FROM_NPC);
		for (int i = 0; i < MAX_PRODUCTS_FROM_NPC; i++)
		{
			if (npc->m_proto->m_product[i] != -1)
			{
				itemIndexProduct[i] = npc->m_proto->m_product[i];

				// 등급 구하기
				int grade = GetGradeProductItem(itemIndexProduct[i]);
				if (grade != -1)
				{
					// 스킬에서 확률 구하기
					switch (grade)
					{
					case 0:
						probProduct[i] = sskill->m_proto->m_num5[sskill->m_level - 1];
						break;	// E
					case 1:
						probProduct[i] = sskill->m_proto->m_num6[sskill->m_level - 1];
						break;	// D
					case 2:
						probProduct[i] = sskill->m_proto->m_num7[sskill->m_level - 1];
						break;	// C
					case 3:
						probProduct[i] = sskill->m_proto->m_num8[sskill->m_level - 1];
						break;	// B
					case 4:
						probProduct[i] = sskill->m_proto->m_num9[sskill->m_level - 1];
						break;	// A
					}

					// 패널티 적용 : 무제한 생산
					if (npc->m_proto->CheckFlag(NPC_ETERNAL))
						probProduct[i] = probProduct[i] * ETERNAL_PENALTY_PROB / 10000;

					// 패널티 적용 : 공유지
					if (npc->GetMapAttr() & MATT_PRODUCT_PUBLIC)
						probProduct[i] = probProduct[i] * PUBLIC_PENALTY_PROB / 10000;
				}
			}
		}

		// 누적 확률 구하기
		for (int i = 1; i < MAX_PRODUCTS_FROM_NPC; i++)
		{
			probProduct[i] += probProduct[i - 1];
		}

		// 성공 결정
		int prob = GetRandom(1, 10000);
		prob = prob * gserver->m_bDoubleProducePercent / 100;

		for (int i = 0; i < MAX_PRODUCTS_FROM_NPC; i++)
		{
			if (prob <= probProduct[i])
			{
				LONGLONG count = 1;

				// 더블 이벤트
				if (gserver->m_bDoubleEvent)
					count = gserver->m_bDoubleProduceNum;

				item = gserver->m_itemProtoList.CreateItem(itemIndexProduct[i], -1, 0, 0, count);

				if (npc->GetMapAttr() & MATT_PRODUCT_PRIVATE)
				{
					if (ch->m_inventory.getMoney() >= produceTax)
					{
						ch->m_inventory.decreaseMoney(produceTax);
					}
					gserver->AddTaxProduceCastle(produceTax / 3);
				}
				break ;
			}
		}

		if (item && item->m_itemProto->getItemTypeIdx() == ITYPE_ETC && item->m_itemProto->getItemSubTypeIdx() == IETC_PRODUCT)
		{
			if (ch->m_inventory.addItem(item))
			{
				// Item LOG
				GAMELOG << init("ITEM_PICK_PRODUCE_RANDOM", ch)
						<< itemlog(item)
						<< end;

				switch (item->m_itemProto->getItemIndex())
				{
				case 152:
					STATISTICS(152, item->Count());
					break;
				case 153:
					STATISTICS(153, item->Count());
					break;
				case 154:
					STATISTICS(154, item->Count());
					break;
				case 155:
					STATISTICS(155, item->Count());
					break;
				case 156:
					STATISTICS(156, item->Count());
					break;
				case 157:
					STATISTICS(157, item->Count());
					break;
				case 158:
					STATISTICS(158, item->Count());
					break;
				case 159:
					STATISTICS(159, item->Count());
					break;
				case 160:
					STATISTICS(160, item->Count());
					break;
				case 161:
					STATISTICS(161, item->Count());
					break;
				case 162:
					STATISTICS(162, item->Count());
					break;
				case 163:
					STATISTICS(163, item->Count());
					break;
				case 197:
					STATISTICS(197, item->Count());
					break;
				case 198:
					STATISTICS(198, item->Count());
					break;
				case 199:
					STATISTICS(199, item->Count());
					break;
				}
			}
			else
			{
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = npc->m_pArea->DropItem(item, npc);
				if (!item)
					return;

				item->m_preferenceIndex = ch->m_index;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, npc, true);
				}
			}

			{
				// 생산 성공시 해당하는 종류에 따라 이펙트 메세지 전송
				CNetMsg::SP rmsg(new CNetMsg);
				switch (npcflag)
				{
				case NPC_MINERAL:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_MINING);
					break;
				case NPC_ENERGY:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_GATHERING);
					break;
				case NPC_CROPS:
					EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_PRODUCE_CHARGE);
					break;
				}
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		else
		{
			if (item)
				delete item;
		}
	}

	// 지속
	if (npc->m_proto->CheckFlag(NPC_ETERNAL))
	{
		npc->m_hp = ch->m_maxHP;
	}

	{
		// 이펙트 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		switch (npcflag)
		{
		case NPC_MINERAL:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_MINING);
			break;
		case NPC_ENERGY:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_CHARGE);
			break;
		case NPC_CROPS:
			EffectProduceMsg(rmsg, ch, npc, MSG_PRODUCE_GATHERING);
			break;
		}
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	// 죽었을때
	if (DEAD(npc))
	{
		if (npc->m_proto->CheckFlag(NPC_ETERNAL)) // 지속
		{
			npc->m_hp = ch->m_maxHP;
		}
		else // 비지속
		{
			// 아이템 드롭
			for (int loopcount = 0; loopcount < MAX_NPC_DROPITEM_LOOP; loopcount++)
			{
				// 1. 드롭할 수 있는 수 범위에서 아이템 선정
				// 2. 그 아이팀의 드롭확률로 드롭여부 결정
				// 3. 드롭
				if (npc->m_proto->m_itemCount > 0)
				{
					CItem* dropItem = NULL;
					int tableindex = GetRandom(0, MAX_NPC_DROPITEM - 1);

					if (npc->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= npc->m_proto->m_itemPercent[tableindex])
						dropItem = npc->m_pArea->DropItem(npc->m_proto->m_item[tableindex], npc, 0, 0, 1, true);

					if (dropItem)
					{
						// Drop Msg 보내기
						dropItem->m_preferenceIndex = -1;
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, npc, dropItem);
						npc->m_pArea->SendToCell(rmsg, npc);
					}
				}
			}

			// 어택리스트에서 삭제
			DelAttackList(npc);

			// npc 삭제
			ch->m_pArea->CharFromCell(npc, true);
			ch->m_pArea->DelNPC(npc);
		}
	}
}

