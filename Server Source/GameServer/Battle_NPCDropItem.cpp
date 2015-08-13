#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "Log.h"
#include "hardcoding.h"
#include "Battle.h"

// npc - 사망 npc, pc - 공격한 캐릭터, tpc - 우선권 캐릭터, level - npc를 공격한 캐릭터의 평균레벨
typedef void (*NPC_DROP_FUNCTION) (CNPC* npc, CPC* pc, CPC* tpc, int level);
void ProcDropItemAfterBattle(CNPC* df, CPC* opc, CPC* tpc, int level)
{
	if(df->m_pZone->IsComboZone())
	{
		DropComboGiftMob(df, opc, tpc, level);
		return ;
	}

#ifdef SYSTEM_TREASURE_MAP
	if( df->m_idNum == TREASURE_BOX_NPC_INDEX ) // npc 일경우 보물 상자만 드랍 한다.
	{
		DropTreasureBoxNpc(df, opc, tpc, level);
		return;
	}

	// 보물 지도는 필드에서만 드랍된다.
	if( df->m_pZone->CheckTreasureDropFlag() && df->m_pZone->IsFieldZone() )
	{
		DropTreasureMap(df, opc, tpc, level);
	}
#endif

	// 아이템 드롭율 증폭제 검사
	bool hcSepDrop = false;
	bool hcDropPlus_S360 = false;
	bool hcPlatinumDrop = false;

	if (opc && tpc == opc)
	{
		if (opc->m_assist.m_avAddition.hcSepDrop)
		{
			opc->m_assist.CureByItemIndex(884);	// 아이템
			hcSepDrop = true;
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_DROP);
			SEND_Q(rmsg, opc->m_desc);
		}

		if (opc->m_assist.m_avAddition.hcSepDrop_Cash)
		{
			opc->m_assist.CureByItemIndex(6096);	// 아이템
			hcSepDrop = true;
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_DROP);
			SEND_Q(rmsg, opc->m_desc);
		}

		// 추천서버포션 드롭율 상승
		else if (opc->m_assist.m_avAddition.hcDropPlus_S360)
		{
			opc->m_assist.CureBySkillIndex(360);
			hcDropPlus_S360 = true;
		}
		else if(opc->m_assist.m_avAddition.hcPlatinumDrop)
		{
			opc->m_assist.CureByItemIndex(2855);	// 아이템
			hcPlatinumDrop = true;
		}
	} // 아이템 드롭율 증폭제 검사

	// 아이템 드롭
	int loopcount;
	bool IsNotDropInCube = true;
#ifdef EXTREME_CUBE
	if(df->m_bCubeRegen)
		IsNotDropInCube = false;
#endif // EXTREME_CUBE]

	int itemDropLoop = MAX_NPC_DROPITEM_LOOP;
#ifdef EVENT_WORLDCUP_2010
	if( df->m_proto->m_index == 1105 )
		itemDropLoop = 1;
#endif // #ifdef EVENT_WORLDCUP_2010

	for (loopcount = 0; loopcount < itemDropLoop; loopcount++)
	{
		// 5레벨보다 크면 아이템 드롭 없음
		// 1. 드롭할 수 있는 수 범위에서 아이템 선정
		// 2. 그 아이팀의 드롭확률로 드롭여부 결정
		// 3. 드롭
#ifdef EVENT_WORLDCUP_2010 // 트라이앵글 볼(1105) 일경우 무조건 아이템을 드랍 한다
		if( ( df->m_proto->m_itemCount > 0 &&  ((level != -1 && level - df->m_level <= 5) ||  !IsNotDropInCube) )
			|| df->m_proto->m_index == 1105)
#else
		if (df->m_proto->m_itemCount > 0 &&  ((level != -1 && level - df->m_level <= 5) ||  !IsNotDropInCube) )
#endif
		{
			CItem* dropItem = NULL;
			int tableindex;
#ifdef EVENT_WORLDCUP_2010
			if( df->m_proto->m_index == 1105 )
			{
				// npc툴 드랍 테이블1,2번에 각각 축국공, 황금 축구공이  100%g확률로 들어가 있어야 한다.
				tableindex = GetRandom(1, 10000); // 0. 95%확률로 축구공. 1. 5% 확률 황금 축구공 2. 꽝
				if( tableindex <= 9000 )
					tableindex = 0;
				else if( tableindex <= 9500)
					tableindex = 1;
				else
					tableindex = 2;
			}
			else
#endif
				tableindex = GetRandom(0, MAX_NPC_DROPITEM - 1);
			int dropprob = df->m_proto->m_itemPercent[tableindex];

			dropprob = dropprob * gserver->m_itemDropProb / 100;

			if (tpc)
			{
				// 자두
				if (tpc->m_assist.m_avAddition.hcDropPlus_838)
					dropprob *= 2;

				// 행운의스크롤, 5080강운의 스크롤과 변수를 같이 사용한다.
				if (tpc->m_assist.m_avAddition.hcScrollDrop)
					dropprob *= 2;

				if (tpc->m_assist.m_avAddition.hcScrollDrop_5081)
					dropprob *= 4;

				// 행운 주문서
				if (tpc->m_assist.m_avAddition.hcDropPlus_2141)
				{
					if (GetRandom(1, 10000) <= 2000 ) // 20 %
					{
						dropprob *= 2;
					}
				}

				// 플래티늄 행운의 스크롤
				if(tpc->m_assist.m_avAddition.hcPlatinumScroll)
				{
					dropprob *= 4;
				}
			}

			// BS 수정 : 아이템 드롭 이벤트
			dropprob = dropprob * gserver->m_nItemDropEventRate / 100;

			// 드롭율 상승 %단위 누적
			if (opc && tpc == opc && opc->m_assist.m_avAddition.hcDropPlusPer100 > 0)
				dropprob += dropprob * opc->m_assist.m_avAddition.hcDropPlusPer100 / 100;

#ifdef DOUBLE_ITEM_DROP
			if ( gserver->m_bDoubleItemEvent )
				dropprob += dropprob * gserver->m_bDoubleItemPercent / 100;
#endif // DOUBLE_ITEM_DROP

			// 9월 이벤트 드롭율 10배
			if (hcSepDrop)
				dropprob = dropprob * 10;

			// 추천서버포션 드롭율 상승
			else if (hcDropPlus_S360)
				dropprob = dropprob * 10;

			else if (hcPlatinumDrop)
				dropprob = dropprob * 20;

			if( opc && opc->m_assist.m_avAddition.hcRandomDropUp > 0  && GetRandom(0,100) <= opc->m_assist.m_avAddition.hcRandomDropUp )
			{
				dropprob = dropprob * 10;
				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg(rmsg, opc, MSG_EFFECT_ETC_RANDOM_DROP);
				opc->m_pArea->SendToCell(rmsg, opc, true);
			}

			if( tpc )
			{
				if( gserver->isActiveEvent( A_EVENT_XMAS) )
				{
					if ( tpc->m_assist.m_avAddition.hcDropPlus_Xmas2007 > 0)
						dropprob += df->m_proto->m_itemPercent[tableindex] * tpc->m_assist.m_avAddition.hcDropPlus_Xmas2007;
				}
			}

#ifdef IMP_SPEED_SERVER
			// Zone Drop 률 적용
			if( gserver->m_bSpeedServer && tpc && tpc->m_pZone )
			{
				dropprob = dropprob * tpc->m_pZone->GetZoneDrop() / 100;
			}
#endif //IMP_SPEED_SERVER

			if (df->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= dropprob)
			{
				if (df->m_proto->m_item[tableindex] == 84)
				{
					dropItem = df->m_pArea->DropItem(df->m_proto->m_item[tableindex], df, 0, df->m_level, 1);
				}
#if defined (LC_USA) || defined(LC_BILA)
				// 대만과 말레이시아는 중소형 체력약이 소형으로 떨어진다
				else if( df->m_proto->m_item[tableindex] == 44
						 || df->m_proto->m_item[tableindex] == 45 )
				{
					dropItem = df->m_pArea->DropItem(43, df, 0, 0, 1);
				}
				// 대만과 말레이시아는 중형 마나 회복 물약이 드롭되지 않는다.
				else if ( df->m_proto->m_item[tableindex] == 485 )
				{
					dropItem = NULL;
				}
#endif // #if defined (LC_USA) || defined(LC_BILA)
#if defined (LC_USA) || defined(LC_BILA)
				// 대형 힐링포션 제작서, 대형 마나, 중형 마나 메뉴얼 드롭 금지
				else if ( df->m_proto->m_item[tableindex] == 1066
						  || df->m_proto->m_item[tableindex] == 1067
						  || df->m_proto->m_item[tableindex] == 1068
						  || df->m_proto->m_item[tableindex] == 489)
				{
					dropItem = NULL;
				}
#endif // #if defined (LC_USA) || defined (LC_BILA)
				else
				{
					// 61레벨 무기류 및 65레벨 방어구 드롭 금지 제작서 드롭 금지
					switch (df->m_proto->m_item[tableindex])
					{
					case -1:	// 지우지 말것
						dropItem = NULL;
						break;

					default:
						{
							bool bAvailableDrop = true;
							if (bAvailableDrop)
								dropItem = df->m_pArea->DropItem(df->m_proto->m_item[tableindex], df, 0, 0, 1, true);
							else
								dropItem = NULL;
						}
						break;
					} // switch (df->m_proto->m_item[tableindex])
				}
			} // if (df->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= dropprob)

			if (dropItem)
			{
// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
				if (df->m_proto->m_minplus >= 0 && df->m_proto->m_maxplus >= df->m_proto->m_minplus && df->m_proto->m_probplus > 0 && dropItem->CanUpgrade())
				{
					if (GetRandom(1, 10000) <= df->m_proto->m_probplus)
					{
						dropItem->setPlus(GetRandom(df->m_proto->m_minplus, df->m_proto->m_maxplus));
					}
				}

				// Drop Msg 보내기
				// 아이템 우선권 셋팅 (같은 데미지 고려, 선공 고려)
				if (tpc)
					dropItem->m_preferenceIndex = tpc->m_index;
				else
					dropItem->m_preferenceIndex = -1;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, df, dropItem);
					df->m_pArea->SendToCell(rmsg, GET_YLAYER(dropItem), dropItem->m_cellX, dropItem->m_cellZ);
				}

				if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
				{
					GAMELOG << init("MOB DROP ITEM")
							<< "NPC INDEX" << delim
							<< df->m_proto->m_index << delim
							<< "NPC NAME" << delim
							<< df->m_name << delim
							<< "ITEM" << delim
							<< itemlog(dropItem)
							<< end;
				}
			}
		}
	} // // 아이템 드롭

	// opc 공격 캐릭터, tpc 우선권 캐릭터
	int job = -1;

	if(tpc)
		job = (int)tpc->m_job;
	else if(opc)
		job = (int)opc->m_job;

	if(job >= 0 && job < JOBCOUNT)
	{
		if(level != -1 && level - df->m_level <= 5)
		{
			if(df->m_proto->m_jobdropitem[job] > 0 && df->m_proto->m_jobdropitemprob[job] > 0)
			{
				int dropprob = df->m_proto->m_jobdropitemprob[job];
				if(GetRandom(1, 10000) <= dropprob)
				{
					CItem* dropItem = NULL;
					dropItem = gserver->m_itemProtoList.CreateItem(df->m_proto->m_jobdropitem[job], -1, 0, 0, 1);
					if(dropItem)
					{
						df->m_pArea->DropItem(dropItem, df);
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, df, dropItem);
						df->m_pArea->SendToCell(rmsg, df, true);
						GAMELOG << init("MOB DROP ITEM")
								<< "NPC INDEX" << delim
								<< df->m_proto->m_index << delim
								<< "NPC NAME" << delim
								<< df->m_name << delim
								<< "ITEM" << delim
								<< itemlog(dropItem)
								<< end;
					}
				}
			}
		}
	}

	// 이 아이템은 레벨제한 없이 무조건 떨어트린다.
	int loopi = 0;
	for(loopi = 0; loopi < MAX_NPC_DROPITEM; loopi++)
	{
		if(df->m_proto->m_dropallitem[loopi] < 1)
			continue ;

		int dropprob = df->m_proto->m_dropallitemprob[loopi];
		if(GetRandom(1, 10000) <= dropprob)
		{
			CItem* pItem = NULL;

			pItem = gserver->m_itemProtoList.CreateItem(df->m_proto->m_dropallitem[loopi], -1, 0, 0, 1);
			if(pItem)
			{
				if (tpc)
					pItem->m_preferenceIndex = tpc->m_index;
				else
					pItem->m_preferenceIndex = -1;

				df->m_pArea->DropItem(pItem, df);
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, df, pItem);
				df->m_pArea->SendToCell(rmsg, df, true);
				GAMELOG << init("MOB DROP ALL ITEM")
						<< "NPC INDEX" << delim
						<< df->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< df->m_name << delim
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
			}
		}
	}
	// 보석 드롭
	for (loopcount = 0; loopcount < MAX_NPC_DROPITEM_LOOP; loopcount++)
	{
		// 5레벨보다 크면 아이템 드롭 없음
		// 1. 드롭할 수 있는 수 범위에서 아이템 선정
		// 2. 그 아이팀의 드롭확률로 드롭여부 결정
		// 3. 드롭
		if (df->m_proto->m_jewelCount > 0 &&  ((level != -1 && level - df->m_level <= 5) ||  !IsNotDropInCube)  )
		{
			CItem* dropItem = NULL;
			int tableindex = GetRandom(0, MAX_NPC_DROPJEWEL - 1);
			int dropprob = df->m_proto->m_jewelPercent[tableindex];

			if (df->m_proto->m_jewel[tableindex] != -1 && GetRandom(1, 10000) <= dropprob)
			{
				switch (df->m_proto->m_jewel[tableindex])
				{
				case -1:	// 지우지 말것
					dropItem = NULL;
					break;

				default:
					{
						dropItem = df->m_pArea->DropItem(df->m_proto->m_jewel[tableindex], df, 0, 0, 1, true);
					}
					break;
				}
			}

			if (dropItem)
			{
				// Drop Msg 보내기
				// 아이템 우선권 셋팅 (같은 데미지 고려, 선공 고려)
				if (tpc)
					dropItem->m_preferenceIndex = tpc->m_index;
				else
					dropItem->m_preferenceIndex = -1;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemDropMsg(rmsg, df, dropItem);
					df->m_pArea->SendToCell(rmsg, GET_YLAYER(dropItem), dropItem->m_cellX, dropItem->m_cellZ);
				}

				GAMELOG << init("JEWEL ITEM")
						<< "NPC INDEX" << delim
						<< df->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< df->m_name << delim
						<< "ITEM" << delim
						<< itemlog(dropItem)
						<< end;
			}
		}
	} // 보석 드롭
	// typedef void (*NPC_DROP_FUNCTION) (CNPC* npc, CPC* pc, CPC* tpc, int level);
	// pc, tpc는 NULL이 될 수 있다
	NPC_DROP_FUNCTION fnNPCDrop[] =
	{
		DropBloodGem,						// 블러드젬 드롭 : 대만 천하대란
		DropLuckySpecialStone,				// 행운의 제련석 드롭 : 대만 천하대란
		DropSpecialRefineStone,				// 고제 드롭
		DropPersonalDungeon2Ticket,			// 퍼스널던전 2 입장권 드롭
		DropBoosterItem,					// 부스터

		DropPersonalDungeon3Ticket,			// 퍼스널던전 3 입장권 드롭
		DropPersonalDungeon4Ticket,			// 퍼스널던전 4 입장권 드롭

		DropPetEgg,							// 애완동물 알 드롭

		DropNewMoonStoneItem,

#ifdef EVENT_VALENTINE
		DropValentineItem,					// 발렌타인
#endif

#ifdef EVENT_WHITEDAY
		DropWhiteDayItem,					// 화이트데이
#endif

#ifdef DROP_MAKE_DOCUMENT
		DropMakeDocument,					// 제작문서 드롭
#endif // DROP_MAKE_DOCUMENT

		DropRecommendItem,					// 추천 서버 전용 인스턴스 포션 아이템

		DropGoldenBallItem,					// 골든볼 이벤트

		RegenBlessWarrior,					// 전사의 축복

		DropHalloween2006Item,				// 2006 할로윈 이벤트

		DropRaidMonsterItem,

		DropMobScrollSpecialStone,

		DropEventGomdori2007,

		DropEventIndependenceDay2007USA,

		DropEventAprilFoolEvent,

#ifdef EVENT_DROPITEM
		DropEventNpcDropItem,
#endif // EVENT_DROPITEM

		DropAPetLifeBook,

		DropPhoenix_MembersTicket,

		DropTriggerItem,

#ifdef LACARETTE_SYSTEM
		DropLacaRette,
#endif
		DropHolyWater,
		DropWorldCupEvent,
		DropHalloween2014Event,
		DropArtifactItem,

#ifdef DEV_EVENT_AUTO
		DropEventItem,
#endif // DEV_EVENT_AUTO
	}; // 드롭 함수 테이블

	// 방어코드 : tpc가 있는데 m_pZone이나 m_pArea가 없으면 NULL로 바꾼다
	if (tpc && (tpc->m_pZone == NULL || tpc->m_pArea == NULL))
		tpc = NULL;

	unsigned int fnDropLoop;
	for (fnDropLoop = 0; fnDropLoop < sizeof(fnNPCDrop) / sizeof(NPC_DROP_FUNCTION); fnDropLoop++)
		(fnNPCDrop[fnDropLoop])(df, opc, tpc, level);

	gserver->doEventDropItem(df, opc, tpc);

	// 돈 떨어뜨릴 확률 : default 80 %
	// BS 수정 : 낮은 레벨 몬스터 잡을때 패널티만 존재
	// 잠수함 : 돈 드롭확률
	int moneyDropProb = MONEY_DROP_PROB * gserver->m_moneyDropProb / 100;
	if (level != -1 && df->m_level - level < 0)
		moneyDropProb += (df->m_level - level) * 500;

	bool hcSepNas = false;
	if (opc && tpc == opc)
	{
		if (opc->m_assist.m_avAddition.hcSepNas)
		{
			opc->m_assist.CureByItemIndex(885);	// 나스
			hcSepNas = true;
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_NAS);
			SEND_Q(rmsg, opc->m_desc);
		}
	}

	// 돈 드롭
	//CItem* money = NULL;
	if (GetRandom(1, 10000) <= moneyDropProb)	// 80%
	{
		// 돈 액수 : +- 50%
		if(!df)
		{
			//잘못된 위치에서 몬스터를 잡았을 경우
			GAMELOG << "NOT FOUND TARGET....."
					<< df->m_idNum
					<< end;
			return ;
		}
		GoldType_t count = df->m_proto->m_price * GetRandom(50, 150) / 100;

#ifdef LC_RUS
		// 러시아는 망각의 신전을 제외한 지역에서 65 레벨 이상의 몬스터가 떨구는 나스는 40% 로 줄인다.
		if (df->m_pZone && df->m_pZone->m_index != ZONE_DUNGEON4 && df->m_level >= 65)
			count = count / 5 * 2;
#endif // LC_RUS

		// 더블이벤트
		if (gserver->m_bDoubleEvent)
		{
#ifdef NEW_DOUBLE_GM_ZONE
			if( gserver->m_bDoubleEventZone == -1 || gserver->m_bDoubleEventZone == df->m_pZone->m_index )
#endif // NEW_DOUBLE_GM_ZONE
				count = count * gserver->m_bDoubleNasPercent / 100 ;
		}

		// 9월 이벤트 나스 10배
		if (hcSepNas)
			count = count * 10;

		// 행운 주문서
		if (tpc && tpc->m_assist.m_avAddition.hcDropPlus_2141)
		{
			if (GetRandom(1, 10000) <= 8000 ) // 80 %
			{
				count *= 2;
			}
		}

//#endif // CAHNCE_EVENT

		if( opc && opc->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON )
		{
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if( opc->m_guildInfo && opc->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex()  )
			{
				// 세금 없음
			}
			else
			{
				GoldType_t tax=0;
				tax = count * pCastle->m_dvd.GetHuntRate() / 100;
				count = count - tax;
				gserver->AddTaxItemDratan( tax );
			}
		}

		if( tpc == NULL )
		{
			GAMELOG << "NOT FOUND TARGET....." << end;
			return;
		}

		//파티이면서 타입이 균등일때
		if (tpc->IsParty() && (tpc->m_party->GetPartyType(MSG_DIVITYPE_MONEY) == MSG_PARTY_TYPE_RANDOM || tpc->m_party->GetPartyType(MSG_DIVITYPE_MONEY) == MSG_PARTY_TYPE_BATTLE) )
		{
			DivisionPartyMoney(tpc, count);
		}
		//원정대이면서 타입이 균등일때
		else if ( tpc->IsExped() && (tpc->m_Exped->GetExpedType(MSG_DIVITYPE_MONEY) == MSG_EXPED_TYPE_RANDOM || tpc->m_Exped->GetExpedType(MSG_DIVITYPE_MONEY) == MSG_EXPED_TYPE_BATTLE) )
		{
			DivisionExpedMoney(tpc, count);
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
				bonus = count * (tpc->m_avPassiveRate.money_nas - 100) / SKILL_RATE_UNIT;
			}

			count = count + count * tpc->m_artiGold / 100;
			
			tpc->m_inventory.increaseMoney(count, bonus);
		}

		if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
		{
			GAMELOG << init("MOB DROP MONEY")
					<< "NPC INDEX" << delim
					<< df->m_proto->m_index << delim
					<< "NPC NAME" << delim
					<< df->m_name << delim
					<< "MONEY(NAS)" << delim
					<< count
					<< end;
		}
	}
}
//