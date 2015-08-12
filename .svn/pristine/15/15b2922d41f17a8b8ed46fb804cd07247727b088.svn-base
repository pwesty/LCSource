#include "stdhdrs.h"

#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "hardcoding.h"
#include "Battle.h"
#include "Artifact_Manager.h"

void DropRaidMonsterItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if(!npc || !(npc->m_proto) || !(npc->m_pArea))
		return;

#ifdef BUGFIX_DROPITEM_LEVEL_CHECK
	if(!tpc)
		return ;
	if(tpc->m_level - npc->m_proto->m_level > 5)
		return ;
#endif // BUGFIX_DROPITEM_LEVEL_CHECK

	CItem*			pItem = NULL;

	// 레이드 드롭 데이터 처리
	vec_raiddropdata_t::iterator it = npc->m_proto->m_listRaidDrop.begin();
	vec_raiddropdata_t::iterator endit = npc->m_proto->m_listRaidDrop.end();
	for(; it != endit; ++it)
	{
		RAIDDROPDATA& rdd = *(it);

		//레이드 일반 아이템 드랍
		if (rdd.itemindex > 0 && rdd.itemindex != NAS_ITEM_DB_INDEX)
		{
			// 확률 적용
			int nProb = GetRandom(1, 10000);
			if (nProb <= rdd.prob)
			{
				pItem = npc->m_pArea->DropItem(rdd.itemindex, npc, 0, rdd.flag, rdd.count, true);
				if (pItem && pItem->m_pArea)
				{
					if(tpc)
					{
						pItem->m_preferenceIndex = tpc->m_index;
					}
					else
					{
						pItem->m_preferenceIndex = -1;
					}
#ifdef TLD_EVENT_SONG
					if(npc->m_idNum == 1623)
					{
						pItem->m_preferenceIndex = -1;
					}
#endif
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, npc, pItem);
						pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
					}

					// 7. 게임 로그
					GAMELOG << init("RAID MOB DROP")
							<< "NPC" << delim
							<< npc->m_idNum << delim
							<< npc->m_name << delim
							<< "NPC LEVEL" << delim
							<< npc->m_level << delim
							<< "PC" << delim;
					if (tpc)
						GAMELOG << tpc->m_index << delim << tpc->m_name << delim << tpc->m_nick << delim << tpc->m_desc->m_idname << delim;
					else
						GAMELOG << -1 << delim << "(none)" << delim << "(none)" << delim << "(none)" << delim;
					GAMELOG << "PC LEVEL" << delim
							<< level << delim
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;
				}
			}
		}
		else if(rdd.itemindex == NAS_ITEM_DB_INDEX)
		{
			int nProb = GetRandom(1, 10000);
			if (nProb <= rdd.prob)
			{
				if( tpc == NULL )
				{
					GAMELOG << "NOT FOUND TARGET....." << end;
					return;
				}
				//파티이면서 타입이 균등
				if (tpc->IsParty() && (tpc->m_party->GetPartyType(MSG_DIVITYPE_MONEY) == MSG_PARTY_TYPE_RANDOM || tpc->m_party->GetPartyType(MSG_DIVITYPE_MONEY) == MSG_PARTY_TYPE_BATTLE) )
				{
					DivisionPartyMoney(tpc, rdd.count);
				}
				//원정대이면서 타입이 균등일때
				else if ( tpc->IsExped() && (tpc->m_Exped->GetExpedType(MSG_DIVITYPE_MONEY) == MSG_EXPED_TYPE_RANDOM || tpc->m_Exped->GetExpedType(MSG_DIVITYPE_MONEY) == MSG_EXPED_TYPE_BATTLE) )
				{
					DivisionExpedMoney(tpc, rdd.count);
				}
				//이도 저도 아닐때 (개인 플레이할때)
				else
				{
					int bonus = 0;

					if(tpc->m_avPassiveAddition.money_nas > 0)
					{
						bonus += tpc->m_avPassiveAddition.money_nas;	
					}
					if(tpc->m_avPassiveRate.money_nas > 0)
					{
						bonus = rdd.count * (tpc->m_avPassiveRate.money_nas - 100) / SKILL_RATE_UNIT;
					}

					rdd.count = rdd.count + rdd.count * tpc->m_artiGold / 100;

					tpc->m_inventory.increaseMoney(rdd.count, bonus);
				}
			}
		}
	}

	// 스패셜 상자 드롭 처리
	RAIDDROPDATA rdd;

	if (npc->m_proto->m_listRaidDrop.empty() == false)
	{
		rdd = npc->m_proto->m_listRaidDrop[0];
	}
	else
	{
		rdd.itemindex=0;
	}

	if (rdd.itemindex > 0)
	{
		if(pc && pc->IsParty() && pc->m_party->GetPartyType(MSG_DIVITYPE_SPECIAL)==MSG_PARTY_TYPE_OPENBOX)
		{
			//파티 상자 열기
			DropRaidBoxItem(npc, pc, MSG_JOINTYPE_PARTY, npc->m_pArea);
			return;
		}

		if( (pc && pc->IsExped() && pc->m_Exped->GetExpedType(MSG_DIVITYPE_SPECIAL)==MSG_EXPED_TYPE_OPENBOX)
				|| (pc && pc->IsExped() && pc->m_Exped->GetExpedType(MSG_DIVITYPE_SPECIAL) == MSG_EXPED_TYPE_FIRSTGET)
		  )
		{
			//원정대 상자 열기
			DropRaidBoxItem(npc, pc, MSG_JOINTYPE_EXPED, npc->m_pArea);
			return;
		}
		else
		{
			return ;
		}
	}
	else
	{
		return ;
	}
}

//파티, 원정대 스페셜 아이템 - 상자열기- 상자 아이템 드랍
void DropRaidBoxItem(CNPC* npc, CPC* tpc, int IsPartyExped, CArea* pArea)
{
	if(!npc || !tpc || !pArea || !npc->m_proto)
		return;

	int i;
	int nBoxItemIndex = 4709;		//BOX 아이템 인덱스

	// 몬스터가 그랜드 이면 스페셜 상자는 스킵한다.
	if(npc->m_proto->CheckFlag(NPC_RAID))
		return;

	if(IsPartyExped == MSG_JOINTYPE_PARTY)
	{
		//파티원수 만큼
#ifdef RAIDBOX_DROP_COUNT_MODIFY
		// tpc가 파티가 있는지 없는지는 함수호출전 검사 했음.
		CArea* pArea = NULL;
		pArea = tpc->m_pArea;
		CPartyMember* pMember = NULL;
		CPC* pPC = NULL;
		int nDropCount = 0;
		for(i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			pMember = tpc->m_party->GetMemberByListIndex(i);
			if(pMember && pMember->GetMemberPCPtr())
			{
				pPC = pMember->GetMemberPCPtr();
				if(pPC && pPC->m_pArea == pArea)
					nDropCount++;
			}
		}
		for(i=0; i < nDropCount; i++)
#else // RAIDBOX_DROP_COUNT_MODIFY
		int partymember = tpc->m_party->GetMemberCountOnline();
		for(i=0; i < partymember; i++)
#endif // RAIDBOX_DROP_COUNT_MODIFY
		{
			CItem* pItem = gserver->m_itemProtoList.CreateItem(nBoxItemIndex, -1, 0, 0, 1);
			if(!pItem)
				return;

			pItem->SetDropNpc(npc->m_idNum);
			pItem->m_preferenceIndex = -1;	//박스 아이템 입수 우선

			pArea->DropItem( pItem, tpc );
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, tpc, pItem);
				pArea->SendToCell(rmsg, tpc, true);
			}
		}
	}
	else if(IsPartyExped == MSG_JOINTYPE_EXPED)
	{
		//원정대원수 만큼
#ifdef RAIDBOX_DROP_COUNT_MODIFY
		CArea* pArea = NULL;
		pArea = tpc->m_pArea;
		CPC* pPC = NULL;
		int j = 0;
		int nDropCount = 0;
		for(i = 0; i < MAX_EXPED_GROUP; i++)
		{
			for(j = 0; j < MAX_EXPED_GMEMBER; j++)
			{
				pPC = tpc->m_Exped->GetMemberPCPtr(i, j);
				if(pPC == NULL)
					continue;
				if(pArea == pPC->m_pArea)
					nDropCount++;
			}
		}
		for(i=0; i < nDropCount; i++)
#else // RAIDBOX_DROP_COUNT_MODIFY
		int expedMemberCount = tpc->m_Exped->GetMemberCountOnline();
		for(i=0; i < expedMemberCount; i++)
#endif // RAIDBOX_DROP_COUNT_MODIFY
		{
			CItem* pItem = gserver->m_itemProtoList.CreateItem(nBoxItemIndex, -1, 0, 0, 1);
			if(!pItem)
				return;

			pItem->SetDropNpc(npc->m_idNum);
			pItem->m_preferenceIndex = -1;	//박스 아이템 입수 우선
			{
				pArea->DropItem( pItem, npc );
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, pItem);
				pArea->SendToCell(rmsg, npc, false);
			}
		}
	}
}

void DropGoldenBallItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 1. 이벤트 진행 상태 검사 : GOLDENBALL_STATUS_VOTE
	// 2. tpc 검사
	// 3. 퍼던 제외
	// 4. 확률 계산 : 1/10000, 기본 180, -4부터 0.2%씩 감소
	// 5. 확률 적용
	// 6. 아이템 드롭
	// 7. 게임로그

	// 1. 이벤트 진행 상태 검사 : GOLDENBALL_STATUS_VOTE
	if (gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE)
		return ;

	// 2. tpc 검사
	if (tpc == NULL)
		return ;

	// 3. 퍼던 제외
	if (tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 4. 확률 계산 : 1/10000, 기본 180, -4부터 0.2%씩 감소
	int nProb = 180;
	if (tpc->m_level - npc->m_level >= 4)
		nProb -= (tpc->m_level - npc->m_level - 3) * 20;

	// 5. 확률 적용
	if (GetRandom(1, 10000) <= nProb)
	{
		// 6. 아이템 드롭
		CItem* pItem = gserver->m_itemProtoList.CreateItem(GOLDENBALL_BALL_INDEX, -1, 0, 0, 1);
		if (!pItem)
			return ;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
		}

		// 7. 게임로그
		GAMELOG << init("EVENT GOLDEN BALL DROP", tpc)
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

void DropRecommendItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 추천 서버에서
	// 우선권 갖는 PC가 있고
	// 파란색 몹을 잡은 게 아니고
	// 30레벨 이하일 경우
	// 20%의 확률로
	if (!gserver->m_bRecommend)
		return ;
	if (!tpc)
		return ;
	if (tpc->m_level - npc->m_level > 5)
		return ;
	if (tpc->m_level > RECOMMEND_SERVER_LEVEL)
		return ;
	if (GetRandom(1, 100) > 20)
		return ;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(RECOMMEND_SERVER_POTION_INDEX, -1, 0, 0, 1);
	if (!pItem)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
		GAMELOG << init("DROP RECOMMEND SERVER POTION", tpc)
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

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventRecommendPotionMsg(rmsg);
		SEND_Q(rmsg, tpc->m_desc);
	}
}

#ifdef DROP_MAKE_DOCUMENT
void DropMakeDocument(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	int nNPCLevel = npc->m_level;
	int i;
	void* pos;
	int nSelItem = -1;

	if (!tpc)
		return ;

	// npc.level < 10 이면 드롭 안함
	if (nNPCLevel < 10)
		return ;

	// tpc.level - npc.level > 2 이면 드롭안함
	if (tpc->m_level - nNPCLevel > 2)
		return ;

	// NPC 레벨별 확률 구하기 : {minLevel, maxLevel, prob(1/1000)}
	const int nSizeTable = 4;
	int tableProb[nSizeTable][3] =
	{
		{10, 20, 50},		// Lv.10 ~ Lv.20, 5%
		{21, 30, 30},		// Lv.21 ~ Lv.30, 5%
		{31, 40, 20},		// Lv.31 ~ Lv.40, 5%
		{41, 50, 10}		// Lv.41 ~ Lv.50, 5%
	};
	for (i = 0; i < nSizeTable; i++)
	{
		if (tableProb[i][0] <= npc->m_level && npc->m_level <= tableProb[i][1])
		{
			if (GetRandom(1, 1000) <= tableProb[i][2])
				break;
			else
				return ;
		}
	}
	if (i == nSizeTable)
		return ;

	std::vector<int> listItemIndex;

	// 해당 레벨 대의 아이템 구하기
	for (i = 0; i < gserver->m_itemProtoList.m_nCount; i++)
	{
		const CItemProto* pItemProto = gserver->m_itemProtoList.m_protoItems + i;
		if (pItemProto)
		{
			// item.level - 5 <= npc.level <= item.level - 2 일경우만
			if (pItemProto->getItemTypeIdx() == 2 && pItemProto->getItemSubTypeIdx() == 2 && pItemProto->GetItemProtoLevel() - 5 <= npc->m_level && npc->m_level <= pItemProto->GetItemProtoLevel() - 2)
			{
				listItemIndex.push_back(pItemProto->getItemIndex());
			}
		}
	}

	// 해당 레벨 대의 아이템이 없으면 드롭 없음
	if (listItemIndex.empty())
		return ;

	// 특정 아이템 선정 : 각 아이템 선정 확률은 동일
	i = GetRandom(0, listItemIndex.size() - 1);
	nSelItem = listItemIndex[i];

	// 해당 아이템 드롭
	CItem* pItem = gserver->m_itemProtoList.CreateItem(nSelItem, -1, 0, 0, 1);
	if (pItem)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
		GAMELOG << init("DROP MAKE DOCUMENT", tpc)
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
#endif // DROP_MAKE_DOCUMENT

void DropLuckySpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
}

void DropBloodGem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
}

void DropBoosterItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#if defined (LC_TLD) || defined (LC_KOR)
#else
	if (level != -1 && level - npc->m_level <= 5 && npc->m_level >= 6 && !npc->m_pZone->IsPersonalDungeon())
	{
		int prob = 54;
		if (npc->m_level >= 31)
			prob -= (level - 30) * 2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(786, npc, 0, 0, 1);
			if (item)
			{
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}

				GAMELOG << init("MOB DROP BOOSTER")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
#endif // #ifndef LC_TLD
}

// 인비지 푸는 아이템 사용시
int ApplyItem677_or_3579(CCharacter* ch, CSkill* skill, int itemidx)
{
	CArea* area = ch->m_pArea;

	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;

	int x, z;
	int count = skill->m_proto->m_levelproto[skill->m_level - 1]->m_targetNum;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= area->m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= area->m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = area->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (GetDistance(ch, pChar) <= skill->m_proto->m_appRange)
				{
					if (pChar->IsInvisible() )
					{
						if(IS_NPC(pChar) && TO_NPC(pChar)->GetOwner() && TO_NPC(pChar)->Check_MobFlag(STATE_MONSTER_TRAP) )
						{
							if( TO_NPC(pChar)->IsEnemy(ch) || IsRaList(TO_PC(ch),TO_NPC(pChar)->GetOwner() ) )
							{
								count--;
								bool bApply;
								ApplySkill(ch, pChar, skill, itemidx, bApply);
								if (count < 1)
									return 0;
							}
							continue;
						}
						else
						{
							count--;
							bool bApply;
							ApplySkill(ch, pChar, skill, itemidx, bApply);
							if (count < 1)
								return 0;
						}
					}
				}
			}
		}
	}

	return 0;
}

// 퍼스널 던전3 입장권 주기
void DropPersonalDungeon3Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	CQuestProto* proto = NULL;
	if (!pc)
		return ;

	// pc level이 30이상
	proto = gserver->m_questProtoList.FindProto(104);
	if (proto == NULL)
		return ;

	if (pc->m_level < proto->m_needMinLevel || pc->m_level > proto->m_needMaxLevel )
		return ;

	// 확률 : 5%
	if (GetRandom(1, 100) > 5)
		return ;

	// 입장이후 하루 지났는가?
	if (pc->m_pd3Count >= 5)
		return ;

	// 입장권을 소지하고 있나?
	if (pc->m_inventory.FindByDBIndex(725, 0, 0))
		return ;

	// 레벨 차이는? : pc->level - npc->level <= 5
	if (pc->m_level - npc->m_level > 5)
		return ;

	// 아이템 생성 후 인벤토리에 넣기
	CItem* item = gserver->m_itemProtoList.CreateItem(725, -1, 0, 0, 1);
	if (!item)
		return ;

	if (pc->m_inventory.addItem(item) == false)
	{
		delete item;
		return ;
	}
}
//#endif

void DropPersonalDungeon4Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	CQuestProto* proto = NULL;
	if (!pc)
		return ;

	proto = gserver->m_questProtoList.FindProto(105);

	if (proto == NULL)
		return ;

	if (pc->m_level < proto->m_needMinLevel || pc->m_level > proto->m_needMaxLevel )
		return ;

	// 확률 : 5%
	if (GetRandom(1, 100) > 5)
		return ;

	// 입장이후 하루 지났는가?
	// 그외 3회
	if (pc->m_pd4Count >= 3)
		return;

	// 입장권을 소지하고 있나?
	if (pc->m_inventory.FindByDBIndex(757, 0, 0))
		return ;

	// 레벨 차이는? : pc->level - npc->level <= 5
	if (pc->m_level - npc->m_level > 5)
		return ;

	// 아이템 생성 후 인벤토리에 넣기
	CItem* item = gserver->m_itemProtoList.CreateItem(757, -1, 0, 0, 1);
	if (!item)
		return ;

	if (pc->m_inventory.addItem(item) == false)
	{
		delete item;
		return ;
	}
}
//#endif

void DropPetEgg(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
#ifdef LC_TLD
		int prob = 100;

		if( tpc )
		{
			// 판의 피리나 드레이크 알을 가지고 있으면 드롭하지 않는다
			// or 펫이 있거나
			if (tpc->m_inventory.FindByDBIndex(948, 0, 0) || tpc->m_inventory.FindByDBIndex(949, 0, 0) || tpc->m_petList )
				return;
		}
#else
		int prob = 30;
#endif
		if (prob < 0)
			return ;

		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		int ran = rand()%2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(948+ran, npc, 0, 0, 1);
			if (item)
			{
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}

				GAMELOG << init("MOB_DROP_PETEGG")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}

// 고제드롭
void DropSpecialRefineStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if( npc->m_level > 140 )
		return;

	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = ((npc->m_level * 2) + 25) / 10;
		if (prob < 0)
			return ;

		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;
#ifdef LC_TLD // 태국 고제 드롭율 10프로 증가
		prob += prob * 10 / 100;
#endif

#if defined(LC_BILA)// 말레시아 고제 트롭율 50% 감소		/// kj
		prob /=  2;
#endif

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(85, npc, 0, 0, 1);
			if (item)
			{
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}

				GAMELOG << init("MOB DROP SPECIALREFINE")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}

#ifdef EVENT_WHITEDAY
void DropWhiteDayItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver->m_bWhiteDayEvent)
	{
		if (level != -1 && level - npc->m_level <= 5)
		{
			int prob;

			if (npc->m_pZone->IsPersonalDungeon())
				prob = 200;
			else
				prob = 600;

			// 사탕 드롭
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(556, npc, 0, 0, 1);
				if (item)
				{
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;

					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif

#ifdef EVENT_VALENTINE
void DropValentineItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver->m_bValentineEvent)
	{
		if (level != -1 && level - npc->m_level <= 5)
		{
			int prob;

			if (npc->m_pZone->IsPersonalDungeon())
				prob = 200;
			else
				prob = 600;

			// 초컬릿 드롭
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(512, npc, 0, 0, 1);
				if (item)
				{
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;

					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif

void DropPersonalDungeon2Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (npc->m_level >= S2_TICKET_MOB_START && npc->m_level <= S2_TICKET_MOB_END)
	{
		if (GetRandom(1, 10000) <= S2_TICKET_DROP_PROB)
		{
			CItem* ticket = npc->m_pArea->DropItem(gserver->m_itemProtoList.m_sTicketItem2->getItemIndex(), npc, 0, 0, 1, false);

			if (ticket)
			{
				if (tpc)
					ticket->m_preferenceIndex = tpc->m_index;
				else
					ticket->m_preferenceIndex = -1;

				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, ticket);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(ticket), ticket->m_cellX, ticket->m_cellZ);
			}
		}
	}
}

void DropNewMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		// 잠수함 : 문스톤 드롭
		int prob = 90;

		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		// 문스톤상자 드롭
		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(2544, npc, 0, 0, 1);
			if (item)
			{
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;

				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}

void RegenBlessWarrior(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	static const int		nProbWarrior		= 10;					// 전사의 손길 확률 : 10%
	static const int		nProbGuard			= 20 + nProbWarrior;	// 수호의 손길 확률 : 20%
	static const int		nProbBrave			= 35 + nProbGuard;		// 용맹의 손길 확률 : 35%
	static const int		nProbProtect		= 35 + nProbBrave;		// 보호의 손길 확률 : 35%

	static const int		nIndexWarrior		= 310;					// 전사의 손길
	static const int		nIndexGuard			= 311;					// 수호의 손길
	static const int		nIndexBrave			= 312;					// 용맹의 손길
	static const int		nIndexProtect		= 313;					// 보호의 손길

	if (pc == NULL && tpc == NULL)
		return ;
	if (tpc == NULL)
		tpc = pc;
	else if (tpc->m_level > level)
		level = tpc->m_level;

	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

#ifdef BUGFIX_DROPITEM_LEVEL_CHECK
	if(tpc->m_level - npc->m_proto->m_level > 5)
		return ;
#endif // BUGFIX_DROPITEM_LEVEL_CHECK

	int nProb = 6;		// 기본 확률 : 6/1000 : 0.6%

	if (npc->m_level < level)
		nProb -= level - npc->m_level;

	if (GetRandom(1, 1000) <= nProb)
	{
		nProb = GetRandom(1, 100);

		int nNPCIndex = 0;

		if (nProb <= nProbWarrior)			nNPCIndex = nIndexWarrior;
		else if (nProb <= nProbGuard)		nNPCIndex = nIndexGuard;
		else if (nProb <= nProbBrave)		nNPCIndex = nIndexBrave;
		else if (nProb <= nProbProtect)		nNPCIndex = nIndexProtect;

		if (nNPCIndex != 0)
		{
			CNPC*		pNPCBless = gserver->m_npcProtoList.Create(nNPCIndex, NULL);
			if (pNPCBless)
			{
				GET_X(pNPCBless)			= GET_X(npc);
				GET_Z(pNPCBless)			= GET_Z(npc);
				GET_R(pNPCBless)			= GET_R(npc);
				GET_YLAYER(pNPCBless)		= GET_YLAYER(npc);

				pNPCBless->m_regenX			= GET_X(pNPCBless);
				pNPCBless->m_regenZ			= GET_Z(pNPCBless);
				pNPCBless->m_regenY			= GET_YLAYER(pNPCBless);

				pNPCBless->m_recoverPulse	= gserver->m_pulse;

				pNPCBless->InitPointsToMax();
				pNPCBless->m_disableTime	= 0;

				npc->m_pArea->AddNPC(pNPCBless);

				int		nCellX;
				int		nCellZ;
				npc->m_pArea->PointToCellNum(GET_X(pNPCBless), GET_Z(pNPCBless), &nCellX, &nCellZ);
				npc->m_pArea->CharToCell(pNPCBless, GET_YLAYER(pNPCBless), nCellX, nCellZ);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					AppearMsg(rmsg, pNPCBless, true);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(pNPCBless), nCellX, nCellZ);
				}

				pNPCBless->m_aipulse		= gserver->m_pulse;
				pNPCBless->m_delay			= PULSE_REAL_SEC * 2;
				pNPCBless->m_nBlockRegen	= tpc->m_index;
			}
		}
	}
}

void DropBlessWarrior(CNPC* pNPC)
{
	for (int i = 0; i < MAX_NPC_DROPITEM; i++)
	{
		if (pNPC->m_proto->m_item[i] > 0)
		{
			int			nItemIndex			= pNPC->m_proto->m_item[i];
			CItem*		pItem				= NULL;

			switch (nItemIndex)
			{
			case 84:		// 일반 제련석은 무시
				break;

			default:
				pItem = pNPC->m_pArea->DropItem(nItemIndex, pNPC, 0, 0, 1, true);
				break;
			}

			if (pItem)
			{
				pItem->m_preferenceIndex = pNPC->m_nBlockRegen;

				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, pNPC, pItem);
				pNPC->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

				GAMELOG << init("BLESS OF WARRIOR DROP")
						<< "PC INDEX" << delim
						<< pNPC->m_nBlockRegen << delim
						<< "NPC" << delim
						<< pNPC->m_idNum << delim
						<< pNPC->m_name << delim
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
			}
		}
	}

	pNPC->SendDisappearAllInCell(true);

	// 어택리스트에서 삭제
	DelAttackList(pNPC);

	// npc 삭제
	pNPC->m_pArea->CharFromCell(pNPC, true);
	pNPC->m_pArea->DelNPC(pNPC);
}

void DropHalloween2006Item( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !gserver->isActiveEvent(A_EVENT_PANDORA_BOX) )
		return;

	if ( !tpc )
		return;

	if( tpc->m_pZone->IsPersonalDungeon() )
		return;

	if( npc->m_proto->m_index != 314 )
		return;

	int nProb = 0;
	//nProb = gsever.m_HalloweenEventPercent;
	nProb = GetRandom( 1, 1000 );

	if( nProb <= 300 )
	{
		CPC* givePc = pc;
		if ( tpc != NULL || pc != tpc )
			givePc = tpc;

		gserver->m_dropItemMgr.doDrop(givePc, npc, 11 );
	}
	else if( nProb > 300 && nProb <= 600 )
	{
		CNPC* TempNPC = NULL;
		if( tpc->m_pZone->m_index == ZONE_START )
		{
			TempNPC = gserver->m_npcProtoList.Create( 315, NULL );
		}
		else if( tpc->m_pZone->m_index == ZONE_DRATAN )
		{
			TempNPC = gserver->m_npcProtoList.Create( 317, NULL );
		}
		else if( tpc->m_pZone->m_index == ZONE_MERAC )
		{
			TempNPC = gserver->m_npcProtoList.Create( 319, NULL );
		}

		if( !npc )
			return;
		int nTry = 0;
		do
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(TempNPC), GET_X(TempNPC), GET_Z(TempNPC)) & MATT_WALKABLE)
				break;
			nTry++;
		}
		while( nTry <= 10 );

		if (nTry > 10)
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		TempNPC->m_regenX = GET_X(TempNPC);
		TempNPC->m_regenY = GET_YLAYER(TempNPC);
		TempNPC->m_regenZ = GET_Z(TempNPC);

		int cx, cz;
		npc->m_pArea->AddNPC(TempNPC);
		npc->m_pArea->PointToCellNum(GET_X(TempNPC), GET_Z(TempNPC), &cx, &cz);
		npc->m_pArea->CharToCell(TempNPC, GET_YLAYER(TempNPC), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, TempNPC, true);
			npc->m_pArea->SendToCell(rmsg, GET_YLAYER(TempNPC), cx, cz);
		}
	}
	else
	{
		CNPC* TempNPC = NULL;
		if( tpc->m_pZone->m_index == ZONE_START )
		{
			TempNPC = gserver->m_npcProtoList.Create( 316, NULL );
		}
		else if( tpc->m_pZone->m_index == ZONE_DRATAN )
		{
			TempNPC = gserver->m_npcProtoList.Create( 318, NULL );
		}
		else if( tpc->m_pZone->m_index == ZONE_MERAC )
		{
			TempNPC = gserver->m_npcProtoList.Create( 320, NULL );
		}
		if( !npc )
			return;
		int nTry = 0;
		do
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(TempNPC), GET_X(TempNPC), GET_Z(TempNPC)) & MATT_WALKABLE)
				break;
			nTry++;
		}
		while( nTry <= 10 );

		if (nTry > 10)
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		TempNPC->m_regenX = GET_X(TempNPC);
		TempNPC->m_regenY = GET_YLAYER(TempNPC);
		TempNPC->m_regenZ = GET_Z(TempNPC);

		int cx, cz;
		npc->m_pArea->AddNPC(TempNPC);
		npc->m_pArea->PointToCellNum(GET_X(TempNPC), GET_Z(TempNPC), &cx, &cz);
		npc->m_pArea->CharToCell(TempNPC, GET_YLAYER(TempNPC), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, TempNPC, true);
			npc->m_pArea->SendToCell(rmsg, GET_YLAYER(TempNPC), cx, cz);
		}
	}
}

void DropMobScrollSpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc || !npc)
		return ;

	if (npc->m_MobScrollType != -1
			&& npc->m_UseCharIndex != -1)
	{
		static const int patterns[2][13] =
		{
			{1, 297, 0, 9, 133, 132, 264, 125, 126, 158, 68, 69, 250,},	// 2144
			{2, 190, 267, 8, 250, 251, 252, 189, 188, 236, 190, 189, 0,},// 2145
		};

		int pattern_index;
		int npc_index;
		int temp;

		if (npc->m_NextMobIndex == 9)
		{
			// 몬스터 소환서로 나온 보스 몬스터
			if (GetRandom(1, 10000) <= 3000)	// 30% 확률
			{
				CItem* item = gserver->m_itemProtoList.CreateItem(85, -1, 0, 0, 1);	// 고급 제련석
				if (item)
				{
					item->m_preferenceIndex = npc->m_UseCharIndex;
					npc->m_pArea->DropItem(item, npc);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, npc, item);
						item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
					}

					GAMELOG << init("MOB SCROLL SPECAIL STONE ITEM DROP", tpc)
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
				}
			}

			npc->m_MobScrollType = -1;
			npc->m_NextMobIndex = -1;
			npc->m_UseCharIndex = -1;

			return;
		}
		else if (npc->m_NextMobIndex == 8)
		{
			pattern_index = npc->m_MobScrollType;
			temp = GetRandom(1, patterns[pattern_index][0]);
			npc_index = patterns[pattern_index][temp];
		}
		else
		{
			pattern_index = npc->m_MobScrollType;
			temp = GetRandom(4, 4 + patterns[pattern_index][3] - 1);
			npc_index = patterns[pattern_index][temp];
		}

		CNPCProto * proto = gserver->m_npcProtoList.FindProto(npc_index);
		if (!proto)
		{
			return;
		}

		CNPC* npc_new = gserver->m_npcProtoList.Create(npc_index, NULL);
		if (!npc_new)
			return;

		npc_new->m_NextMobIndex = npc->m_NextMobIndex + 1;
		npc_new->m_UseCharIndex = npc->m_UseCharIndex;
		npc_new->m_MobScrollType = npc->m_MobScrollType;

		int nTry = 0;
		do
		{
			GET_X(npc_new) = GET_X(tpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_YLAYER(npc_new) = GET_YLAYER(tpc);
			GET_Z(npc_new) = GET_Z(tpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_R(npc_new) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(npc_new), GET_X(npc_new), GET_Z(npc_new)) & MATT_WALKABLE)
				break;
			nTry++;
		}
		while (nTry <= 10);
		if (nTry > 10)
		{
			GET_X(npc_new) = GET_X(npc);
			GET_Z(npc_new) = GET_Z(npc);
			GET_YLAYER(npc_new) = GET_YLAYER(npc);
			GET_R(npc_new) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		npc_new->m_regenX = GET_X(npc_new);
		npc_new->m_regenY = GET_YLAYER(npc_new);
		npc_new->m_regenZ = GET_Z(npc_new);

		int cx, cz;
		tpc->m_pArea->AddNPC(npc_new);
		tpc->m_pArea->PointToCellNum(GET_X(npc_new), GET_Z(npc_new), &cx, &cz);
		tpc->m_pArea->CharToCell(npc_new, GET_YLAYER(npc_new), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, npc_new, true);
			tpc->m_pArea->SendToCell(rmsg, GET_YLAYER(npc_new), cx, cz);
		}

		npc_new->m_postregendelay += 2 * PULSE_REAL_SEC;
	}
}

void DropEventGomdori2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if( !gserver->isActiveEvent(A_EVENT_BEAR_DOLL) )
		return;

#if defined(LC_BILA)
	const static int nProbHigh = 1;
	const static int nProbLowPerLevel = 0;
	const static int nProbLimit = 9;
#else
	const static int nProbHigh			= 80;		// 높은 레벨 잡으면 80%
	const static int nProbLowPerLevel	= 10;		// 낮은 레벨 잡으면 레벨당 10%
	const static int nProbLimit			= 9;		// 9레벨 이하 레벨 잡으면 0
#endif //

	if (npc->m_pZone->IsPersonalDungeon())
		return ;

	// 우선순위 : tpc, level, pc
	if (tpc)
	{
		level = tpc->m_level;
	}
	else
		return ;

	// 드롭 결정
	// 1. 시간 검사 : 매시 x9분 또는 x4분
	// 2. 레벨에 따른 변경 : 높은 몹은 80%, 낮은 몹은 레벨당 -10%, 9레벨 이하 드롭 안함
#if defined(LC_BILA)
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 4:
	case 9:
		if(tpc->m_bEventGomdori2007Drop < gserver->m_pulse)
			nDropProb = nProbHigh;
		break;
	}
#else
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 4:
	case 9:
		if (tpc && tpc->m_bEventGomdori2007Drop)
			nDropProb = 100;
		break;
	default:
		tpc->m_bEventGomdori2007Drop = true;
		break;
	}
#endif // LC_BILA

	if (nDropProb == 0)
		return ;
	if (level < npc->m_level)
	{
		nDropProb = nProbHigh;
	}
	else if (level > npc->m_level)
	{
		if (level - npc->m_level >= nProbLimit)
			nDropProb = 0;
		else
			nDropProb -= (level - npc->m_level) * nProbLowPerLevel;
	}
	else if (level == npc->m_level)
	{
		nDropProb = nProbHigh;
	}

	if (nDropProb >= GetRandom(1, 100))
	{
#if defined(LC_BILA)
		tpc->m_bEventGomdori2007Drop = gserver->m_pulse + PULSE_REAL_HOUR * 2;
#else
		tpc->m_bEventGomdori2007Drop = false;
#endif // LC_BILA
		if( tpc )
			gserver->m_dropItemMgr.doDrop(tpc, npc, 10 );
		else if ( pc )
			gserver->m_dropItemMgr.doDrop(pc, npc, 10 );
	}
}

void DropEventAprilFoolEvent(CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !gserver->isActiveEvent(A_EVENT_APRIL_FOOL))
		return;

	if(!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return ;

	const static int nDropProbMax = 80;
	const static int nDiffDropLevel = 9;

	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 0:
	case 9:
		if (tpc && tpc->m_bEventIndependenceDay2007drop)
			nDropProb = 100;
		break;
	default:
		tpc->m_bEventIndependenceDay2007drop = true;
		break;
	}

	if(nDropProb == 0)
		return ;

	if(tpc->m_level < npc->m_level)
		nDropProb = nDropProbMax;
	else
	{
		if(tpc->m_level - npc->m_level < nDiffDropLevel)
			nDropProb -= (tpc->m_level - npc->m_level) * 10;
		else
			return ;
	}

	if(GetRandom(1, 100) > nDropProb)
		return;

	tpc->m_bEventIndependenceDay2007drop = false;

	//int nItemArrayIndex = 0;
	int nItemProb = GetRandom(1, 100);

	if( nItemProb <= 5 )
	{
		gserver->m_dropItemMgr.doDrop( tpc, npc, 22 );
	}
	else
	{
		if( nItemProb <= 70 )
			gserver->m_dropItemMgr.doDrop( tpc, npc, 23, -1, 0 );
		else
			gserver->m_dropItemMgr.doDrop( tpc, npc, 23, -1, 1 );
	}
}

void DropEventIndependenceDay2007USA(CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !gserver->isActiveEvent( A_EVENT_INDEPENDENCE_DAY))
		return;

	if(!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return ;

	const static int nDropProbMax = 80;
	const static int nDiffDropLevel = 9;
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 0:
	case 9:
		if (tpc && tpc->m_bEventIndependenceDay2007drop)
			nDropProb = 100;
		break;
	default:
		tpc->m_bEventIndependenceDay2007drop = true;
		break;
	}

	if(nDropProb == 0)
		return ;

	if(tpc->m_level < npc->m_level)
		nDropProb = nDropProbMax;
	else
	{
		if(tpc->m_level - npc->m_level < nDiffDropLevel)
			nDropProb -= (tpc->m_level - npc->m_level) * 10;
		else
			return ;
	}

	if(GetRandom(1, 100) > nDropProb)
		return;

	tpc->m_bEventIndependenceDay2007drop = false;

	//int nItemArrayIndex = 0;
	int nItemProb = GetRandom(1, 100);

	if( nItemProb <= 5 )
	{
		gserver->m_dropItemMgr.doDrop( tpc, npc, 13 );
	}
	else
	{
		if( nItemProb <= 65 )
			gserver->m_dropItemMgr.doDrop( tpc, npc, 12, -1, 0 );
		else
			gserver->m_dropItemMgr.doDrop( tpc, npc, 12, -1, 1 );
	}
}

#ifdef EVENT_DROPITEM
void DropEventNpcDropItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if(!gserver->m_bDropItem)
		return ;

	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;

	if(npc->m_proto->m_index != gserver->m_bDropItemNpcIdx)
		return ;

	int prob;
	int diffProb = 1;
	prob = gserver->m_itemDropProb;

	if( npc->m_level < tpc->m_level )
	{
		int diffLevel = tpc->m_level - npc->m_level;
		prob -= diffLevel * diffProb;
	}

	if (GetRandom(1, 1000) <= prob )			// 1%의 확률
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(gserver->m_bDropItemIdx, -1, 0, 0, 1);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT NPC DROPITEM", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // EVENT_DROPITEM

void DropComboGiftMob( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !tpc )
		return ;

	if( !tpc->m_pZone->IsComboZone() )
		return ;

	// 선물상자 npc인지 검사
	if(npc->m_proto->m_index == 492)
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(npc->m_coinidx, -1, 0, 0, npc->m_coincount);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("DROP_MONSTER_COMBO_COIN", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}

void DropAPetLifeBook( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !tpc )
		return ;

	if( tpc->m_pZone->IsComboZone() || tpc->m_pZone->IsPersonalDungeon() )
		return ;

	const int nChapterCount=2;
	int nLifeBookIndex[nChapterCount] = { 2947, 2948, };

	if( GetRandom( 1, 1000 ) == 777 )
	{
		CItem* pItem = gserver->m_itemProtoList.CreateItem( nLifeBookIndex[GetRandom(0,nChapterCount-1)] , -1, 0, 0, 1 );
		if( !pItem )
			return;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( pItem, npc );
			ItemDropMsg( rmsg, npc, pItem );
			pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );
		}

		GAMELOG << init("APET LIFE BOOK ITEM DROP ", tpc )
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog( pItem )
				<< end;
	}
}

void DropPhoenix_MembersTicket( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 이벤트 아이템 드랍
	if (!tpc || !npc || !npc->m_pArea || !tpc->m_pZone)
		return ;

	// 동굴 존이 아니면 드랍하지 않는다.
	if( tpc->m_pZone->m_index != ZONE_SPRIT_CAVE
			&& tpc->m_pZone->m_index != ZONE_QUANIAN_CAVE
			&& tpc->m_pZone->m_index != ZONE_GOLEM_CAVE
			&& tpc->m_pZone->m_index != ZONE_TRIVIA_CANYON )
		return;

	// 가입권 드랍
	if( GetRandom(1, 10000) <= DROP_PHOENIX_MEMBERS_TICKET )
	{
		CItem* pItem = gserver->m_itemProtoList.CreateItem( 3217, -1, 0, 0, 1 );
		if( !pItem )
			return;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( pItem, npc );
			ItemDropMsg( rmsg, npc, pItem );
			pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );
		}

		GAMELOG << init("EVENT_PHOENIX TICKET ITEM DROP ", tpc )
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog( pItem )
				<< end;
	}
}

void DropTriggerItem( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if (!tpc || !npc || !npc->m_pArea || !tpc->m_pZone || !npc->m_proto)
		return ;

	// 트리거를 사용하는 존에서만 드랍을 한다.
	if( !npc->m_pArea->m_bUseTriggerEvent )
		return;

	int itemIndex = 0;

	//
	switch(npc->m_proto->m_index)
	{
	case 954:
		// 회랑 열쇠
		itemIndex = 4697;
		break;

	case 964:
		// 사제관 열쇠
		itemIndex = 4695;
		break;

	case 997:
		// 촉매제
		itemIndex = 4566;
		break;

	case 1015:
		// 만화경
		itemIndex = 4571;
		break;

	case 1016:
		// 독수리의 날개
		itemIndex = 4569;
		break;

	case 1017:
		// 붉은 심장
		itemIndex = 4570;
		break;

	default:
		return;
		break;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(itemIndex, -1, 0, 0, 1);
	if( !pItem )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );
	}

	GAMELOG << init("DROP TRIGGER ITEM", pc) << "ITEM" << delim << itemlog(pItem) << end;
}

#ifdef LACARETTE_SYSTEM
void DropLacaRette(CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if (!tpc)
		return;

	if(tpc->m_pZone->IsPersonalDungeon() )	// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if(npc == NULL)
		return ;

	if(tpc->m_level - npc->m_level >= 10)		// 나의 레벨이 몬스터의 레벨보다 10이상 클때
		return ;

	switch(gserver->m_tRealSystemTime.tm_min)	// 매시간 9분, 19분, 29분, 39분, 49분, 59분 에서 1분간만 드롭되는 시간이다.
	{
	case 9:
	case 19:
	case 29:
	case 39:
	case 49:
	case 59:
		break;
	default:
		return;
		break;
	}

	static const int nIndexItem = 5123; // 일반 토큰

	int nProb = 2000;							// 기본 확률은 20%

	if (npc->m_level < tpc->m_level)			// npc레벨이 나의 레벨보다 작을때 2%/1lv 수치로 확률은 줄어듬.
	{
		nProb -= (tpc->m_level - npc->m_level) * 200;
	}

	int drop_count = 1;

	if (GetRandom( 1, 10000 ) <= nProb )
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, drop_count);
		if( item )
		{
			//item->m_preferenceIndex = tpc->m_index;		//이벤트 일반 토큰 주인 없앰
			npc->m_pArea->DropItem( item, npc );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );
			}

			GAMELOG << init( "EVENT LACARETTE NORMAL TOKEN ITEM DROP", tpc )
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		} // if( item )
	}//if (GetRandom( 1, 10000 ) <= nProb)
}
#endif

void DropHolyWater(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return;

	if(tpc->m_pZone->IsPersonalDungeon() )	// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if(npc == NULL)
		return ;

	if(tpc->m_level - npc->m_level >= 10)		// 나의 레벨이 몬스터의 레벨보다 10이상 클때
		return ;

	static const int nIndexItem = 10295;

	int nProb = 100;							// 기본 확률은 1%
	
	int drop_count = 1;

	if (GetRandom( 1, 10000 ) <= nProb )
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, drop_count);
		if( item )
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( item, npc );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );
			}

			GAMELOG << init( "EVENT LACARETTE NORMAL TOKEN ITEM DROP", tpc )
				<< "ITEM" << delim
				<< itemlog(item)
				<< end;
		}
	}
}

void DropWorldCupEvent(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return;

	if(tpc->m_pZone->IsPersonalDungeon() )	// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if(tpc->m_assist.FindBySkillIndex(1770) == false)
		return;

	if(npc == NULL)
		return ;

	if(tpc->m_level - npc->m_level >= 10)		// 나의 레벨이 몬스터의 레벨보다 10이상 클때
		return ;

	static const int nIndexItem = 5345;

	int nProb = 50;							// 기본 확률은 0.5%

	int drop_count = 1;

	if (GetRandom( 1, 10000 ) <= nProb )
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, drop_count);
		if( item )
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( item, npc );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );
			}

			GAMELOG << init( "EVENT LACARETTE NORMAL TOKEN ITEM DROP", tpc )
				<< "ITEM" << delim
				<< itemlog(item)
				<< end;
		}
	}
}

void DropHalloween2014Event(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return;

	if(tpc->m_pZone->IsPersonalDungeon() )	// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if(tpc->m_assist.FindBySkillIndex(1788) == false)
		return;

	if(gserver->m_eventAutomationMgr.isActiveEvent(A_EVENT_HALLOWEEN) == false)
		return;

	if(npc == NULL)
		return ;

	if(tpc->m_level - npc->m_level >= 10)		// 나의 레벨이 몬스터의 레벨보다 10이상 클때
		return ;

	static const int nIndexItem = 2491;

	int nProb = 50;							// 기본 확률은 0.5%

	int drop_count = 1;

	if (GetRandom( 1, 10000 ) <= nProb )
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, drop_count);
		if( item )
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( item, npc );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );
			}

			GAMELOG << init( "EVENT Halloween Pumpkin item drop", tpc )
				<< "ITEM" << delim
				<< itemlog(item)
				<< end;
		}
	}
}

//유물 아이템 랜덤 드롭
void DropArtifactItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if(gserver->m_bNonPK == true)
		return ;

	if( !gserver->isActiveEvent(A_EVENT_ARTIFACT_HUNTER))
		return ;

	if(ArtifactManager::instance()->getCount() >= ARTIFACT_MAX_ITEM_COUNT)
		return ;

	if (tpc == NULL)
		return;

	if(pc == NULL)
		return;
	
	if(pc->m_pZone == NULL)
		return;

	if(ArtifactManager::instance()->canDrop(pc->m_pZone->m_index) == false)
		return ;
	
	if(npc == NULL && npc->m_pArea == NULL)
		return ;

	if(tpc->m_level < 16)		// 나의 레벨이 몬스터의 레벨보다 10이상 클때
		return ;

	static const int nIndexItem = ARTIFACT_LEVEL1_ITEM_INDEX;

	int nProb = 100;							// 기본 확률은 1%

	int drop_count = 1;

	if (GetRandom( 1, 10000 ) <= nProb )
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, drop_count);
		if( item )
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( item, npc );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );
			}

			GAMELOG << init( "ARTIFACT ITEM DROP", tpc )
				<< "ITEM" << delim
				<< itemlog(item)
				<< end;
		}
	}
}

#ifdef SYSTEM_TREASURE_MAP
void DropTreasureBoxNpc(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	CPC * opc = NULL;

	if( tpc )
		opc = tpc;
	else
		opc = pc;

	if( !opc )
		return;

#ifdef SYSTEM_TREASURE_MAP_GAMIGO
	// 보물지도가 있는지 체크
	if (opc->m_inventory.FindByDBIndex(TREASURE_MAP_ITEM_INDEX) == NULL)
		return;
#endif // SYSTEM_TREASURE_MAP_GAMIGO

	int prob = GetRandom(0, 10000);
	int dropItemIndex = 0;

	if( prob < 3000 )
		dropItemIndex = 6938;
	else
		dropItemIndex = 6937;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(dropItemIndex, -1, 0, 0, 1);
	if (pItem)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		pItem->m_preferenceIndex = opc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		GAMELOG << init("TREASURE_BOX ITEM DROP", opc)
				<< itemlog(pItem)
				<< end;
	}
}

void DropTreasureMap(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	CPC * opc = NULL;

	if( tpc )
		opc = tpc;
	else
		opc = pc;

	if( !opc )
		return;

	if( opc->m_level - npc->m_level > 5 )
		return;

	int i=0;
	for(i=0; i<2; i++)
	{
		int dropItemIndex = 0;
		int prob = 0;

		if( opc->m_level - npc->m_level > 0 )
			prob = (opc->m_level - npc->m_level) * 40;

		switch( i )
		{
		case 0: // 보물 지도
			prob = 200 - prob; // 2% 확률로 드랍
			if( GetRandom(0, 10000) > prob )
				continue;
			dropItemIndex = TREASURE_MAP_ITEM_INDEX;
			break;
		case 1: // 일반 보물 상자 열쇠
			prob = 1000 - prob; // 10% 확률로 드랍
			if( GetRandom(0, 10000) > prob )
				continue;
			dropItemIndex = TREASURE_KEY_ITEM_INDEX;
			break;
		default:
			return;
		}

		CItem* pItem = gserver->m_itemProtoList.CreateItem(dropItemIndex, -1, 0, 0, 1);

		if(!pItem)
			continue;

		// 보물지도라면 보물상자NPC도 summon 해주고 pItem에 보물지도 정보를 세팅해준다.
		if( pItem->getDBIndex() == TREASURE_MAP_ITEM_INDEX )
		{
			if( !opc->m_pZone->SetTresaureMapInfo(pItem) ) // 우선 npc summon 부터 한다.
			{
				delete pItem;
				pItem = NULL;
				continue;
			}
		}
		if(pItem)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			pItem->m_preferenceIndex = opc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			if( pItem->getDBIndex() == TREASURE_MAP_ITEM_INDEX )
			{
				GAMELOG << init("TREASURE_MAP ITEM DROP", opc)
						<< itemlog(pItem)
						<< end;
			}
		}
	}
}

#endif // SYSTEM_TREASURE_MAP

#ifdef DEV_EVENT_AUTO
void DropEventItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if(gserver->m_fathersDay.getEventStart() == true)
	{
		if(!npc)
			return ;

		CPC * opc = NULL;

		if( tpc )
			opc = tpc;
		else
			opc = pc;

		if( !opc )
			return ;
		if( opc->m_pZone->IsPersonalDungeon() )
			return ;

		if( opc->m_level - npc->m_level >= 6 )
			return ;

		CEventKingsBirthdayDropProbTable& dropProb = gserver->m_fathersDay.getItemProbTable();

		int prob = GetRandom(1, 10000);
		if(prob > dropProb.getProb(npc))
			return ;

		prob = GetRandom(1, 10000);
		CEventKingsBirthdayDropItemManager& itemManager = gserver->m_fathersDay.getDropItemManager();
		CItem* item = itemManager.getItem(prob);
		if(item)
		{
			npc->m_pArea->DropItem(item, npc);
			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, npc, item);
			npc->m_pArea->SendToCell(rmsg, npc, true);
			GAMELOG << init("Drop FatherDay Event Item", opc)
					<< "NPC INDEX" << delim
					<< npc->m_proto->m_index << delim
					<< "NPC NAME" << delim
					<< npc->m_proto->m_name << delim
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
	if(gserver->m_DropEvent.getEnable() == true)
	{
		if(!npc)
			return ;

		CPC * opc = NULL;

		if( tpc )
			opc = tpc;
		else
			opc = pc;

		if( !opc )
			return ;
		if( opc->m_pZone->IsPersonalDungeon() )
			return ;

		CDropProbTable& droptable = gserver->m_DropEvent.getDropProbTable();

		int prob = GetRandom(1, 10000);
		if(prob > droptable.getProb(opc, npc))
			return ;

		CDropItemManager& itemManager = gserver->m_DropEvent.getDropItemManager();
		CItem* item = itemManager.getItem();

		if(item)
		{
			npc->m_pArea->DropItem(item, npc);
			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, npc, item);
			npc->m_pArea->SendToCell(rmsg, npc, true);
			GAMELOG << init("Drop Koko Myun Event Item", opc)
					<< "NPC INDEX" << delim
					<< npc->m_proto->m_index << delim
					<< "NPC NAME" << delim
					<< npc->m_proto->m_name << delim
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // DEV_EVENT_AUTO
//
