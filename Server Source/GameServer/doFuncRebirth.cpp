#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

////////////////
// 죽었을때 부활
void do_Rebirth(CPC* ch, CNetMsg::SP& msg)
{
	// 주문서 인덱스 주문서 사용여부, 제자리 부활
	int itemindex = 0;
	char buse = 0;
	char bhere = 0;
	bool bApplyGradeSkill = false;

	if( msg->CanRead(sizeof(itemindex)) )
	{
		RefMsg(msg) >> itemindex
					>> buse
					>> bhere;
	}
	// 죽은 사람은 부활 메시지를 보내면 그 사람의 입력버퍼를 비운다
	if(ch->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
	{
		ch->m_assist.CureBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL);
	}

	if (!DEAD(ch))
	{
		GAMELOG << init("REBIRTH PC FAIL NOT DEAD", ch) << end;
		return;
	}
	else
	{
		GAMELOG << init("REBIRTH PC", ch) << end;
	}

	// 현재 존에 따라 설정
	if (ch->m_pZone == NULL)
		return ;

	int zone = ZONE_START;
	int extra = 0;
	switch (ch->m_pZone->m_index)
	{
	case ZONE_START:
	case ZONE_DUNGEON1:
	case ZONE_DUNGEON2:
	case ZONE_DRATAN:
	case ZONE_SINGLE_DUNGEON_TUTORIAL:
	case ZONE_DUNGEON3:
	case ZONE_MERAC:
	case ZONE_DUNGEON4:
	case ZONE_EGEHA:
	case ZONE_EGEHA_DUNGEON_1:
	case ZONE_DRATAN_CASTLE_DUNGEON:
	case ZONE_COMBO_DUNGEON:
	case ZONE_STREIANA:
	case ZONE_PK_TOURNAMENT:
	case ZONE_SPRIT_CAVE:
	case ZONE_QUANIAN_CAVE:
	case ZONE_GOLEM_CAVE:
	case ZONE_EBONY_MINE:
	case ZONE_MISTY_CANYON:
	case ZONE_FLORAIM_CAVE:
	case ZONE_MONDSHINE:
	case ZONE_CAPPELLA_1:
	case ZONE_CAPPELLA_2:
	case ZONE_ALTER_OF_DARK:
	case ZONE_AKAN_TEMPLE:
	case ZONE_TRIVIA_CANYON:
	case ZONE_TARIAN:
	case ZONE_BLOODYMIR:
	case ZONE_TARIAN_DUNGEON:
		zone = ch->m_pZone->m_index;
		extra = 0;
// 공성진행중 처리
#ifdef JUNO_RENEWAL_MESSAGEBOX
		if((zone == ZONE_START) && (ch->m_etcEvent & ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE)) // 아직 퀘스트를 완료하지 못하였다면
		{
			extra = 6;
			if(ch->m_questList.FindQuest(386, QUEST_STATE_DONE))
			{
				ch->m_etcEvent &= ~ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE;
				extra = 0;
			}
		}
#endif // JUNO_RENEWAL_MESSAGEBOX
		if (ch->m_pZone->m_index == ZONE_MERAC)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
			if (castle && castle->GetState() != WCSF_NORMAL)
			{
				zone = castle->GetZoneIndex();
				extra = castle->GetRegenPoint( ch->GetJoinFlag(castle->GetZoneIndex()), ch);
				if( extra > 0 )
					bApplyGradeSkill = true;
			}
		}

		if (ch->m_pZone->m_index == ZONE_DRATAN)
		{
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if (pCastle->GetState() != WCSF_NORMAL
					&& ch->GetJoinFlag(pCastle->GetZoneIndex()) != WCJF_NONE
					&& (ch->GetMapAttr() & MATT_WAR || ch->m_pZone->IsWarZone((int)ch->m_pos.m_x, (int)ch->m_pos.m_z)) )
			{
				zone = pCastle->GetZoneIndex();
				if (zone == ZONE_DRATAN && itemindex == 0 && buse == 1)
				{
					// 간이 마을
					extra = 27;
				}
				else
				{
					// 부활 진지
					extra = pCastle->GetRegenPoint(ch->GetJoinFlag(pCastle->GetZoneIndex()), ch);
				}
				if( zone == ZONE_DRATAN && extra != 27 )
					bApplyGradeSkill = true;
			}
		}

		break;

	case ZONE_EGEHA_DUNGEON_8:
	case ZONE_EGEHA_DUNGEON_9:
	case ZONE_EGEHA_DUNGEON_10:
		zone = ZONE_EGEHA_DUNGEON_1;
		extra = 0;
		break;

	case ZONE_FREE_PK_DUNGEON:
		zone = ZONE_START;
		extra = 0;
		break;

	case ZONE_SINGLE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON2:
	case ZONE_SINGLE_DUNGEON3:
	case ZONE_SINGLE_DUNGEON4:
		{
			bool bPDQuest = false;
			CQuest* pQuest = NULL;
			const CQuestProto* pQuestProto = NULL;
			int nQuestValue = 0;

			CQuest* pQuestNext = NULL;
			pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				switch (pQuest->GetQuestType0())
				{
				case QTYPE_KIND_DEFEAT:
				case QTYPE_KIND_SAVE:
					bPDQuest = true;
					pQuestProto = pQuest->GetQuestProto();
					nQuestValue = pQuest->GetQuestValue(0);
					break;
				default:
					break;
				}
				if (bPDQuest)
					break;
			}

			if (bPDQuest)
			{
				////////////////////////////////////////
				// helper 실패 보내기

				///////////////////////////////////
				// pd4 퀘스트이면 helper 랭킹에 등록 : BW
				//
				// char index를 보냄
				if (pQuestProto != NULL)
				{
					if( pQuestProto->m_index == 105)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						rmsg->Init(MSG_HELPER_COMMAND);
						RefMsg(rmsg) << MSG_HELPER_PD4_RANK_END
							<< ch->m_index
							<< nQuestValue
							<< 0;

						GAMELOG << init("PD4 END", ch)
							<< nQuestValue << end;

						if (gserver->isRunHelper())
						{
							SEND_Q(rmsg, gserver->m_helper);
						}
						else
						{
							GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
								<< ch->m_index << end;
						}
					}
					else if (pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
					{
						// 아이템 삭제
						pQuest->RemoeQuestItem(ch);
					}

					// Quest Fail Log
					GAMELOG << init("QUEST FAIL", ch)
						<< pQuestProto->m_index
						<< end;
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuestFailMsg(rmsg, pQuest);
					ch->m_questList.DelQuest(ch, pQuest);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			zone = ZONE_START;
			extra = 0;
		}
		break;

	case ZONE_EXTREME_CUBE:
		zone = ZONE_MERAC;
		extra = 0;
		break;

	case ZONE_ROYAL_RUMBLE:
		zone = ZONE_START;
		extra = 0;

		// 전장포인트 1포인트 지급
		ch->AddWarGroundPoint(1);
		// 전장 death 수 1 증가
		ch->AddDeathCount(1);
		break;

	case ZONE_RVR:
		zone = ZONE_RVR;
		if(ch->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
			extra = 1;
		else if(ch->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
			extra = 2;
		else
			extra = 0;
		break;
	case ZONE_OXQUIZROOM:
	case ZONE_GUILDROOM:
	default:
		zone = ZONE_START;
		extra = 0;
		break;
	}

	LONGLONG	nRecoverExp = 0;
	LONGLONG	nRecoverSP  = 0;

	// 주문서에 따라 사망 경험치/ sp 복구

	if( buse && itemindex && ( itemindex == 4933 || ( itemindex == 5958 && ch->m_level < 36)) )	// 부활 주문서 2, 초보자용 부활 주문서 2
	{
		// 사망후 아이템 사용하는 부분
		// 아이템 찾기
		CItem* pItem = ch->m_inventory.FindByDBIndex(itemindex, 0, 0);
		if( pItem )
		{
			// 아이템 제거
			ch->m_inventory.decreaseItemCount(pItem, 1);

			// 경험치 복구
			ch->m_exp += ch->m_loseexp;
			ch->m_skillPoint += ch->m_losesp;
			nRecoverExp = ch->m_loseexp;
			nRecoverSP = ch->m_losesp;
			ch->m_loseexp = 0;
			ch->m_losesp = 0;

			GAMELOG << init("REBIRTH RECOVER EXP SP", ch)
					<< "USE ITEM" << delim << itemindex << delim
					<< "EXP" << delim
					<< nRecoverExp << delim
					<< ch->m_exp << delim
					<< "SP" << delim
					<< nRecoverSP << delim
					<< ch->m_skillPoint
					<< end;
		}
		else
		{
			GAMELOG << init("REBIRTH RECOVER EXP SP", ch)
					<< "USE ITEM" << delim << itemindex << delim
					<< "Loss Exp" << delim << ch->m_loseexp << delim
					<< "Loss SP" << delim << ch->m_losesp << end;
		}
	}
	else if( itemindex && ch->m_assist.FindByItemIndex(itemindex) && buse)
	{
		if(itemindex == 846	|| itemindex == 2667 || itemindex == 7056)
		{
			ch->m_exp += ch->m_loseexp;
			ch->m_skillPoint += ch->m_losesp;
			nRecoverExp = ch->m_loseexp;
			nRecoverSP = ch->m_losesp;
			ch->m_loseexp = 0;
			ch->m_losesp = 0;
		}
		else if(itemindex == 845  // 숙련도복구 주문서
				|| itemindex == 2036) // 럭키 숙련도복구 주문서
		{
			ch->m_skillPoint += ch->m_losesp;
			nRecoverSP = ch->m_losesp;
			ch->m_losesp = 0;
		}
		else if(itemindex == 844  // 경험치복구 주문서
				|| itemindex == 2035) // 럭키 경험치복구 주문서
		{
			ch->m_exp += ch->m_loseexp;
			nRecoverExp = ch->m_loseexp;
			ch->m_loseexp = 0;
		}
		ch->m_assist.CureByItemIndex(itemindex);

		GAMELOG << init("REBIRTH RECOVER EXP SP", ch)
				<< "EXP" << delim
				<< nRecoverExp << delim
				<< ch->m_exp << delim
				<< "SP" << delim
				<< nRecoverSP << delim
				<< ch->m_skillPoint
				<< end;
	}
	else
	{
		bhere = false;
	}

	// HP, MP 회복 하고
	// 대략 반만 회복
	ch->m_hp = ch->m_maxHP / 2;

	// MP는 반이상 있으면 그대로..
	if (ch->m_mp < ch->m_maxMP / 2)
		ch->m_mp = ch->m_maxMP / 2;

	// rebirth MSG 에코 하고
	if (ch->m_pArea)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RebirthMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP | PLAYER_STATE_PKMODE);

// [110630: selo] ITS 3058 사도모드(다크니스)일 때 죽으면 클라로 사도모드 해제되는거 안보내져서 상태 보내기로 수정
	if (ch->m_pArea)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPlayerStateChangeMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	if( bApplyGradeSkill)
		ch->ApplyGuildGradeSkill();

	ch->CalcStatus(true);

	ch->m_bChangeStatus = true;

	if(bhere)
	{
		GoZone(ch,
			   ch->m_pZone->m_index,
			   ch->m_pos.m_yLayer,
			   ch->m_pos.m_x,
			   ch->m_pos.m_z);
	}
	else
	{
		int nSavePoint;
		if (ch->m_pZone->m_index == ZONE_AKAN_TEMPLE)
		{
			nSavePoint = ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].GetSavePoint();
			switch(nSavePoint)
			{
			case 0:
				extra = 0;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				extra = 1;
				break;
			default :
				extra = 0;
				break;
			}
		}

		if (ch->m_pZone->m_index == ZONE_TARIAN_DUNGEON)
		{
			nSavePoint = ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].GetSavePoint();
			switch(nSavePoint)
			{
			case 0:
				extra = 0;
				break;
			default :
				extra = 0;
				break;
			}
		}
		CZone* pZone = gserver->FindZone(zone);
		if (pZone == NULL)
			return;

		if(ch->m_pZone->m_index == ZONE_DUNGEON4 && extra == 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidSceneMsg(rmsg, OBJECT_TYPE_OUTPUT, GROUND_EFFECT_OFF, 1, -1);
			do_ExRaidScene(ch, rmsg);
		}
		GoZone(ch,
			   zone,
			   pZone->m_zonePos[extra][0],
			   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
			   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);

		//무적 버프 (존 이동시에 걸어주는 버프)
		if(ch->IsInRaidZone() == false &&
				pZone->IsPartyRaidZone() == false &&
				pZone->IsPersonalDungeon() == false &&
				pZone->IsPersonalRaid() == false)
		{
			bool bapply;
			CSkill* skill = gserver->m_skillProtoList.Create(IMMOTAL_BUF);
			ApplySkill(ch, ch, skill, -1, bapply);

			//무빙하기전 상태 false로
			ch->m_bFirstDoIt = false;
			//*무적 버프
		}

#ifdef BUGFIX_DEAD_AGAIN_DEAD
		ch->m_deadTime = mktime(&gserver->m_tRealSystemTime);
#endif
		if(ch->m_pZone->m_index == ZONE_AKAN_TEMPLE && nSavePoint == 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, TOUCH_FIELD, 2673, -1);
			do_ExRaidScene(ch, rmsg);
		}
	}
}
